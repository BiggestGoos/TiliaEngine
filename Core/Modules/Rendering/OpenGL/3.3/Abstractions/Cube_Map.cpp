// Vendor
#include "vendor/glad/include/glad/glad.h"

// Standard
#include <stdexcept>
#include <cmath>
#include <iostream>

// Headers
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Cube_Map.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.hpp"
#include "Core/Modules/File_System/Windows/File_System.hpp"
#include "Core/Modules/Exceptions/Tilia_Exception.hpp"
#include "Core/Modules/Console/Logging.hpp"
#include "Core/Values/OpenGL/3.3/Utils.hpp"

tilia::gfx::Cube_Map::Cube_Map()
{

    m_texture_type = enums::Texture_Type::Cube_Map;

    Generate_Texture();

}

tilia::gfx::Cube_Map::Cube_Map(const Cube_Map& cube_map) noexcept
    : Cube_Map()
{

    m_cube_map_data = cube_map.m_cube_map_data;

}

tilia::gfx::Cube_Map::Cube_Map(Cube_Map&& cube_map) noexcept
{

    m_cube_map_data = std::move(cube_map.m_cube_map_data);

    m_ID = cube_map.m_ID;
    cube_map.m_ID = 0;

}

void tilia::gfx::Cube_Map::Reload()
{

    Unbind(true);

    Bind();

    constexpr size_t cube_sides{ *enums::Misc::Cube_Sides };
    for (size_t i = 0; i < cube_sides; i++)
    {

        // Checks for faulty data
        if (   !m_cube_map_data.size 
            || !*m_cube_map_data.sides[i].color_format
            || !*m_cube_map_data.sides[i].data_color_format
            || !m_cube_map_data.sides[i].texture_data)
        {

            utils::Tilia_Exception e{ LOCATION };

            e.Add_Message(
                "Reload of cube map { ID: %v } was stopped due to faulty data"
                "\n>>> Size: %v"
                "\n>>> Format: %v"
                "\n>>> Data Format: %v"
                "\n>>> Side: %v"
                "\n>>> Data: 0x%v"
                )(m_ID)(m_cube_map_data.size)
                (utils::Get_Color_Format_Count(*m_cube_map_data.sides[i].color_format))
                (utils::Get_Color_Format_Count(*m_cube_map_data.sides[i].data_color_format))
                (utils::Get_Cube_Map_Side_String(*enums::Cube_Map_Sides::Positive_X + static_cast<int32_t>(i)))
                (static_cast<void*>(m_cube_map_data.sides[i].texture_data.get()));

            Rebind();

            throw e;

        }

        // Set unpack alignment
        if (m_cube_map_data.sides[i].data_color_format == enums::Data_Color_Format::RGBA)
        {
            GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
        }
        else
        {
            GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
        }

        // Sets filtering and wrapping modes
        GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, *m_cube_map_data.filter_min));
        GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, *m_cube_map_data.filter_mag));
        GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, *m_cube_map_data.wrap_s));
        GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, *m_cube_map_data.wrap_t));
        GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, *m_cube_map_data.wrap_r));

        // Sets openGL data
        try
        {
            GL_CALL(glTexImage2D(*enums::Cube_Map_Sides::Positive_X + static_cast<int32_t>(i), 0,
                *m_cube_map_data.sides[i].color_format,
                m_cube_map_data.size, m_cube_map_data.size, 0,
                *m_cube_map_data.sides[i].data_color_format,
                GL_UNSIGNED_BYTE,
                m_cube_map_data.sides[i].texture_data.get()));
        }
        catch (utils::Tilia_Exception& e)
        {

            e.Add_Message(
                "Cube map { ID: %v } failed to reload"
                "\n>>> Format: %v"
                "\n>>> Data Format: %v"
                "\n>>> Side: %v"
                "\n>>> Size: %v"
                "\n>>> Data: 0x%v"
                "\n>>> Path: %v"
                )(m_ID)
                (utils::Get_Color_Format_Count(*m_cube_map_data.sides[i].color_format))
                (utils::Get_Color_Format_Count(*m_cube_map_data.sides[i].data_color_format))
                (utils::Get_Cube_Map_Side_String(*enums::Cube_Map_Sides::Positive_X + static_cast<int32_t>(i)))
                (m_cube_map_data.size)
                (static_cast<void*>(m_cube_map_data.sides[i].texture_data.get()))
                (m_cube_map_data.sides[i].file_path);

            Rebind();

            throw e;

        }

    }

    Rebind();

}

void tilia::gfx::Cube_Map::Generate_Mipmaps()
{
    constexpr size_t cube_sides{ *enums::Misc::Cube_Sides };
    for (size_t i = 0; i < cube_sides; i++)
    {
        if (!m_cube_map_data.sides[i].texture_data)
        {

            utils::Tilia_Exception e{ LOCATION };

            e.Add_Message(
                "Cube map { ID: %v } could not generate mipmaps because of faulty data"
                "\n>>> Side: %v"
                )(m_ID)
                (utils::Get_Cube_Map_Side_String(*enums::Cube_Map_Sides::Positive_X + static_cast<int32_t>(i)));

            throw e;

        }
    }
    try
    {
        Unbind(true);
        Bind();
        GL_CALL(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
    }
    catch (utils::Tilia_Exception& e)
    {
        e.Add_Message("Cube map { ID: %v } failed to generate mipmaps");

        Rebind();
        
        throw e;
    }
    Rebind();
}

void tilia::gfx::Cube_Map::Set_Filter(const enums::Filter_Size& filter_size, const enums::Filter_Mode& filter_mode)
{
    Unbind(true);
    try
    {
        Bind();
        GL_CALL(glTexParameteri(*m_texture_type, *filter_size, *filter_mode));
    }
    catch (utils::Tilia_Exception& e)
    {

        e.Add_Message(
            "Cube map { ID: %v } failed to set filtering"
            "\n>>> Filter size: %v"
            "\n>>> Filter mode: %v"
            )(m_ID)
            (*filter_size)
            (*filter_mode);

        Rebind();

        throw e;

    }
    Rebind();
    switch (filter_size)
    {
    case enums::Filter_Size::Magnify:
        m_cube_map_data.filter_mag = filter_mode;
        break;
    case enums::Filter_Size::Minify:
        m_cube_map_data.filter_min = filter_mode;
        break;
    }
}

void tilia::gfx::Cube_Map::Set_Wrapping(const enums::Wrap_Sides& wrap_side, const enums::Wrap_Mode& wrap_mode)
{
    Unbind(true);
    try
    {
        Bind();
        GL_CALL(glTexParameteri(*m_texture_type, *wrap_side, *wrap_mode));
    }
    catch (utils::Tilia_Exception& e)
    {

        e.Add_Message(
            "Cube map { ID: %v } failed to set wrapping"
            "\n>>> Wrapping side: %v"
            "\n>>> Wrapping mode: %v"
            )(m_ID)
            (*wrap_side)
            (*wrap_mode);

        Rebind();

        throw e;

    }
    Rebind();
    switch (wrap_side)
    {
    case enums::Wrap_Sides::S:
        m_cube_map_data.wrap_s = wrap_mode;
        break;
    case enums::Wrap_Sides::T:
        m_cube_map_data.wrap_t = wrap_mode;
        break;
    case enums::Wrap_Sides::R:
        m_cube_map_data.wrap_r = wrap_mode;
        break;
    }
}
