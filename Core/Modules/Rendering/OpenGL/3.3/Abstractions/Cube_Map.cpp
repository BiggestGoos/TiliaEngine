// Vendor
#include "vendor/glad/include/glad/glad.h"
#include "vendor/stb_image/include/stb_image/stb_image.h"

// Standard
#include <stdexcept>
#include <cmath>

// Headers
#include "Core/Values/OpenGL/3.3/Utils.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Cube_Map.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.hpp"
#include "Core/Modules/Console/Logging.hpp"
#include "Core/Modules/File_System/Windows/File_System.hpp"
#include "Core/Modules/Exceptions/Tilia_Exception.hpp"

// The file system defined in another file
extern tilia::utils::File_System file_system;

const tilia::render::Cube_Map_Data& tilia::render::Cube_Map_Data::operator=(const Cube_Map_Data& other) noexcept
{
    // Self check
    if (&other == this)
        return *this;
    
    // Copies the data for each side.
    constexpr std::size_t side_count{ *enums::Misc::Cube_Sides };
    for (std::size_t i = 0; i < side_count; i++)
    {
        this->sides[i].file_path = other.sides[i].file_path;
        // If there is any, copies the data of that side
        if (other.sides[i].texture_data) {
            uint32_t byte_count{ static_cast<uint32_t>((powf(static_cast<float>(this->size), 2.0f) * 
            utils::Get_Color_Format_Count(static_cast<uint32_t>(*other.sides[i].data_color_format)))) };
            this->Copy_Data(i, other.sides[i].texture_data.get(), byte_count);
        }
        this->sides[i].color_format      = other.sides[i].color_format;
        this->sides[i].data_color_format = other.sides[i].data_color_format;
    }

    this->size       = other.size;
    this->filter_min = other.filter_min;
    this->filter_mag = other.filter_mag;
    this->wrap_s     = other.wrap_s;
    this->wrap_t     = other.wrap_t;
    this->wrap_r     = other.wrap_r;

    return *this;
}

const tilia::render::Cube_Map_Data& tilia::render::Cube_Map_Data::operator=(Cube_Map_Data&& other) noexcept
{
    // Self check
    if (&other == this)
        return *this;

    // Moves the data for each side.
    constexpr std::size_t side_count{ *enums::Misc::Cube_Sides };
    for (std::size_t i = 0; i < side_count; i++)
    {
        this->sides[i].file_path = std::move(other.sides[i].file_path);
        // If there is any, moves the data of that side
        if (other.sides[i].texture_data) {
            this->sides[i].texture_data.reset(other.sides[i].texture_data.get());
            other.sides[i].texture_data.reset();
        }
        this->sides[i].color_format = other.sides[i].color_format;
        this->sides[i].data_color_format = other.sides[i].data_color_format;
    }

    this->size = other.size;
    this->filter_min = other.filter_min;
    this->filter_mag = other.filter_mag;
    this->wrap_s = other.wrap_s;
    this->wrap_t = other.wrap_t;
    this->wrap_r = other.wrap_r;

    return *this;
}

tilia::render::Cube_Map_Data::Cube_Map_Data(const Cube_Map_Data& other) noexcept
{
    // Self check
    if (&other == this)
        return;

    *this = other;

}

tilia::render::Cube_Map_Data::Cube_Map_Data(Cube_Map_Data&& other) noexcept
{
    // Self check
    if (&other == this)
        return;

    *this = std::move(other);

}

void tilia::render::Cube_Map_Data::Reload(const std::size_t& index)
{

    if (this->sides[index].file_path != "") {
        int32_t temp{};
        int32_t format{};
        this->sides[index].texture_data.reset(file_system.Load_Image(
        this->sides[index].file_path.c_str(), temp, temp, format, 0, true));
        this->sides[index].data_color_format = utils::Get_Data_Color_Format(format);
    }
    else
    {

        utils::Tilia_Exception e{ LOCATION };

        e.Add_Message("Failed to reload data"
            "\n>>> Side: %v"
        )(utils::Get_Cube_Map_Side(*enums::Cube_Map_Sides::Positive_X + static_cast<uint32_t>(index)));

        throw e;

    }

}

void tilia::render::Cube_Map_Data::Reload()
{

    for (std::size_t i = 0; i < *enums::Misc::Cube_Sides; i++)
    {

        if (this->sides[i].file_path != "") {
            int32_t format{};
            this->sides[i].texture_data.reset(file_system.Load_Image
            (this->sides[i].file_path.c_str(),
                this->size,
                this->size,
                format, 0, true));
            this->sides[i].data_color_format = utils::Get_Data_Color_Format(format);
        }
        else
        {

            utils::Tilia_Exception e{ LOCATION };

            e.Add_Message("Failed to reload data"
                "\n>>> Side: %v"
                )(utils::Get_Cube_Map_Side(*enums::Cube_Map_Sides::Positive_X + static_cast<uint32_t>(i)));

            throw e;

        }

    }

}

void tilia::render::Cube_Map_Data::Copy_Data(const std::size_t& index, uint8_t* texture_data, const uint32_t& byte_count)
{
    uint32_t temp_byte_count{ byte_count };
    if (!temp_byte_count) {
        uint32_t byte_count{ static_cast<uint32_t>((powf(static_cast<float>(this->size), 2.0f) *
        utils::Get_Color_Format_Count(static_cast<uint32_t>(*this->sides[index].color_format)))) };
    }
    
    this->sides[index].texture_data = std::make_unique<uint8_t[]>(static_cast<size_t>(temp_byte_count));
    std::copy(texture_data, texture_data + temp_byte_count, this->sides[index].texture_data.get());
}

tilia::render::Cube_Map::Cube_Map()
{

    m_texture_type = enums::Texture_Type::Cube_Map;

    try
    {
        Generate_Texture();
    }
    catch (utils::Tilia_Exception& e)
    {

        e.Add_Message("Cube map { ID: %v } failed to be generated"
        )(m_ID);

        throw e;

    }

}

void tilia::render::Cube_Map::Reload()
{

    Unbind(true);

    // Binds texture
    Bind();

    constexpr size_t cube_sides{ *enums::Misc::Cube_Sides };
    for (size_t i = 0; i < cube_sides; i++)
    {

        if (   !m_cube_map_data.size 
            || !*m_cube_map_data.sides[i].color_format
            || !*m_cube_map_data.sides[i].data_color_format
            || !m_cube_map_data.sides[i].texture_data)
        {

            utils::Tilia_Exception e{ LOCATION };

            e.Add_Message("Reload of cube map { ID: %v } was stopped due to faulty data"
                "\n>>> Size: %v"
                "\n>>> Format: %v"
                "\n>>> Data Format: %v"
                "\n>>> Side: %v"
                "\n>>> Data: 0x%v"
                )(m_ID)(m_cube_map_data.size)
                (utils::Get_Color_Format_Count(*m_cube_map_data.sides[i].color_format))
                (utils::Get_Color_Format_Count(*m_cube_map_data.sides[i].data_color_format))
                (utils::Get_Cube_Map_Side(*enums::Cube_Map_Sides::Positive_X + static_cast<int32_t>(i)))
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

        // Sets pixel data
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

            e.Add_Message("Cube map { ID: %v } failed to reload"
                "\n>>> Format: %v"
                "\n>>> Data Format: %v"
                "\n>>> Side: %v"
                "\n>>> Size: %v"
                "\n>>> Data: 0x%v"
                "\n>>> Path: %v"
                )(m_ID)
                (utils::Get_Color_Format_Count(*m_cube_map_data.sides[i].color_format))
                (utils::Get_Color_Format_Count(*m_cube_map_data.sides[i].data_color_format))
                (utils::Get_Cube_Map_Side(*enums::Cube_Map_Sides::Positive_X + static_cast<int32_t>(i)))
                (m_cube_map_data.size)
                (static_cast<void*>(m_cube_map_data.sides[i].texture_data.get()))
                (m_cube_map_data.sides[i].file_path);

            Rebind();

            throw e;

        }

    }

    // Unbinds texture
    Rebind();

}

/**
 * Generates mipmaps for the openGL texture using glGenerateMipmap
 */
void tilia::render::Cube_Map::Generate_Mipmaps()
{
    Unbind(true);
    constexpr size_t cube_sides{ *enums::Misc::Cube_Sides };
    for (size_t i = 0; i < cube_sides; i++)
    {
        if (!m_cube_map_data.sides[i].texture_data)
        {

            utils::Tilia_Exception e{ LOCATION };

            e.Add_Message("Cube map { ID: %v } could not generate mipmaps because of faulty data"
                "\n>>> Side: %v"
                )(m_ID)
                (utils::Get_Cube_Map_Side(*enums::Cube_Map_Sides::Positive_X + static_cast<int32_t>(i)));

            Rebind();

            throw e;

        }
    }
    try
    {
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

/**
 * Sets the filtering mode for the given size
 */
void tilia::render::Cube_Map::Set_Filter(const enums::Filter_Size& filter_size, const enums::Filter_Mode& filter_mode)
{
    Unbind(true);
    try
    {
        GL_CALL(glTexParameteri(*m_texture_type, *filter_size, *filter_mode));
    }
    catch (utils::Tilia_Exception& e)
    {

        e.Add_Message("Cube map { ID: %v } failed to set filtering"
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

/**
 * Sets the wrapping mode for the given side
 */
void tilia::render::Cube_Map::Set_Wrapping(const enums::Wrap_Sides& wrap_side, const enums::Wrap_Mode& wrap_mode)
{
    Unbind(true);
    try
    {
        GL_CALL(glTexParameteri(*m_texture_type, *wrap_side, *wrap_mode));
    }
    catch (utils::Tilia_Exception& e)
    {

        e.Add_Message("Cube map { ID: %v } failed to set wrapping"
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
