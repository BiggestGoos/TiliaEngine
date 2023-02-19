// Vendor
#include "vendor/glad/KHR_Debug_openGL_3_3/include/glad/glad.h"

// Standard
#include <stdexcept>
#include <cmath>
#include <iostream>

// Headers
#include "Cube_Map.hpp"
#include "Core/Values/Directories.hpp"
#include TILIA_OPENGL_3_3_ERROR_HANDLING_INCLUDE
#include TILIA_WINDOWS_FILE_SYSTEM_INCLUDE
#include TILIA_TILIA_EXCEPTION_INCLUDE
#include TILIA_LOGGING_INCLUDE
#include TILIA_OPENGL_3_3_UTILS_INCLUDE

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

    constexpr size_t cube_sides{ *enums::Geometry_Features::Cube_Faces };
    for (size_t i = 0; i < cube_sides; i++)
    {

        // Checks for faulty data
        if (   !m_cube_map_data.size 
            || !*m_cube_map_data.sides[i].color_format
            || !*m_cube_map_data.sides[i].data_color_format
            || !m_cube_map_data.sides[i].texture_data)
        {

            Rebind();

            throw utils::Tilia_Exception{ utils::Exception_Data{ TILIA_LOCATION } 
                << "Reload of cube map { ID: " << m_ID << " } was stopped due to faulty data"
                << "\n>>> Size: " << m_cube_map_data.size
                << "\n>>> Format: " 
                << utils::Get_Color_Format_Count(*m_cube_map_data.sides[i].color_format)
                << "\n>>> Data Format: " 
                << utils::Get_Color_Format_Count(*m_cube_map_data.sides[i].data_color_format)
                << "\n>>> Side: " 
                << utils::Get_Cube_Map_Side_String(*enums::Cube_Map_Sides::Positive_X +
                    static_cast<int32_t>(i))
                << "\n>>> Data: 0x" 
                << static_cast<void*>(m_cube_map_data.sides[i].texture_data.get()) };

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
        GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, 
            *m_cube_map_data.filter_min));
        GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, 
            *m_cube_map_data.filter_mag));
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
        catch (utils::Tilia_Exception& t_e)
        {

            Rebind();

            t_e.Add_Message(TILIA_LOCATION) 
                << "Cube map { ID: " << m_ID << " } failed to reload"
                << "\n>>> Format: " 
                << utils::Get_Color_Format_Count(*m_cube_map_data.sides[i].color_format)
                << "\n>>> Data Format: " 
                << utils::Get_Color_Format_Count(*m_cube_map_data.sides[i].data_color_format)
                << "\n>>> Side: " 
                << utils::Get_Cube_Map_Side_String(*enums::Cube_Map_Sides::Positive_X +
                    static_cast<int32_t>(i))
                << "\n>>> Size: " << m_cube_map_data.size
                << "\n>>> Data: 0x" 
                << static_cast<void*>(m_cube_map_data.sides[i].texture_data.get())
                << "\n>>> Path: " << m_cube_map_data.sides[i].file_path;

            throw t_e;

        }

    }

    Rebind();

}

void tilia::gfx::Cube_Map::Generate_Mipmaps()
{
    constexpr size_t cube_sides{ *enums::Geometry_Features::Cube_Faces };
    for (size_t i = 0; i < cube_sides; i++)
    {
        if (!m_cube_map_data.sides[i].texture_data)
        {
            throw utils::Tilia_Exception{ utils::Exception_Data{ TILIA_LOCATION } 
                << "Cube map { ID: " << m_ID << " } could not generate mipmaps " 
                << "because of faulty data"
                << "\n>>> Side: " << utils::Get_Cube_Map_Side_String(
                    *enums::Cube_Map_Sides::Positive_X + static_cast<int32_t>(i)) };
        }
    }
    try
    {
        Unbind(true);
        Bind();
        GL_CALL(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
    }
    catch (utils::Tilia_Exception& t_e)
    {
        Rebind();
        t_e.Add_Message(TILIA_LOCATION) 
            << "Cube map { ID: " << m_ID << " } failed to generate mipmaps";
        throw t_e;
    }
    Rebind();
}

void tilia::gfx::Cube_Map::Set_Filter(const enums::Filter_Size& filter_size, 
    const enums::Filter_Mode& filter_mode)
{
    Unbind(true);
    try
    {
        Bind();
        GL_CALL(glTexParameteri(*m_texture_type, *filter_size, *filter_mode));
    }
    catch (utils::Tilia_Exception& t_e)
    {

        Rebind();

        t_e.Add_Message(TILIA_LOCATION)
            << "Cube map { ID: " << m_ID << " } failed to set filtering"
            << "\n>>> Filter size: " << *filter_size
            << "\n>>> Filter mode: " << *filter_mode;;
        throw t_e;
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

void tilia::gfx::Cube_Map::Set_Wrapping(const enums::Wrap_Sides& wrap_side, 
    const enums::Wrap_Mode& wrap_mode)
{
    Unbind(true);
    try
    {
        Bind();
        GL_CALL(glTexParameteri(*m_texture_type, *wrap_side, *wrap_mode));
    }
    catch (utils::Tilia_Exception& t_e)
    {

        Rebind();

        t_e.Add_Message(TILIA_LOCATION)
            << "Cube map { ID: " << m_ID << " } failed to set wrapping"
            << "\n>>> Wrapping side: " << *wrap_side
            << "\n>>> Wrapping mode: " << *wrap_mode;
        throw t_e;
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
