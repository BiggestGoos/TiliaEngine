// Vendor
#include "vendor/glad/include/glad/glad.h"
#include "vendor/stb_image/include/stb_image/stb_image.h"

// Standard
#include <stdexcept>

// Headers
#include "Core/Values/OpenGL/3.3/Utils.h"
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Cube_Map.h"
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.h"
#include "Core/Modules/Console/Logging.h"

const tilia::render::Cube_Map_Data& tilia::render::Cube_Map_Data::operator=(const Cube_Map_Data& other)
{
    // Self check
    if (&other == this)
        return *this;
    
    // Copies the data for each side.
    const std::size_t side_count{ *enums::Misc::Cube_Sides };
    for (std::size_t i = 0; i < side_count; i++)
    {
        this->sides[i].file_path = other.sides[i].file_path;
        // Instead of copying the texture data it is just set to null.
        this->sides[i].texture_data.reset(nullptr);
        this->sides[i].color_format = other.sides[i].color_format;
        this->sides[i].load_color_format = other.sides[i].load_color_format;
    }

    this->size = other.size;
    this->filter_min = other.filter_min;
    this->filter_mag = other.filter_mag;
    this->wrap_s = other.wrap_s;
    this->wrap_t = other.wrap_t;
    this->wrap_r = other.wrap_r;

    return *this;
}

 /**
  * Checks a couple of enums pertaining to the color format, wrapping modes,
  * and filtering modes and sets some C-string with the information. Then
  * prints it all using Log_Indent.
  */ /*
static void Print_Information(tilia::render::Cube_Map& cube_map) 
{

    // Color format C-string
    char format_text[6][6]{};
    // Get color format string
    constexpr const size_t cube_sides{ *tilia::enums::Misc::Cube_Sides };
    for (size_t i = 0; i < cube_sides; i++)
    {
        switch (cube_map.Get_Data().sides[i].color_format)
        {
        case tilia::enums::Color_Format::Red8:
            strcpy_s(format_text[i], 6, "Red8\0");
            break;
        case tilia::enums::Color_Format::RGB8:
            strcpy_s(format_text[i], 6, "RGB8\0");
            break;
        case tilia::enums::Color_Format::RGBA8:
            strcpy_s(format_text[i], 6, "RGBA8");
            break;
        }
    }

    // Filtering C-strings
    char filter_min_text[10]{};
    char filter_max_text[10]{};
    // Gets filtering mode strings
    switch (cube_map.Get_Data().filter_min)
    {
    case tilia::enums::Filter_Mode::Point:
        strcpy_s(filter_min_text, 10, "Point\0");
        break;
    case tilia::enums::Filter_Mode::Bilinear:
        strcpy_s(filter_min_text, 10, "Bilinear\0");
        break;
        //case Filter_Mode::Trilinear:
        //	strcpy_s(filter_min_text, 10, "Trilinear");
        //	break;
    }
    switch (cube_map.Get_Data().filter_mag)
    {
    case tilia::enums::Filter_Mode::Point:
        strcpy_s(filter_max_text, 10, "Point\0");
        break;
    case tilia::enums::Filter_Mode::Bilinear:
        strcpy_s(filter_max_text, 10, "Bilinear\0");
        break;
    }
    // Wrapping C-string
    char wrap_s_text[14]{};
    char wrap_t_text[14]{};
    char wrap_r_text[14]{};
    // Gets wrapping mode strings
    switch (cube_map.Get_Data().wrap_s)
    {
    case tilia::enums::Wrap_Mode::Repeat:
        strcpy_s(wrap_s_text, 14, "Repeat\0");
        break;
    case tilia::enums::Wrap_Mode::Mirror_Repeat:
        strcpy_s(wrap_s_text, 14, "Mirror_Repeat");
        break;
    case tilia::enums::Wrap_Mode::Clamp_Edge:
        strcpy_s(wrap_s_text, 14, "Clamp_Edge\0");
        break;
    }
    switch (cube_map.Get_Data().wrap_t)
    {
    case tilia::enums::Wrap_Mode::Repeat:
        strcpy_s(wrap_t_text, 14, "Repeat\0");
        break;
    case tilia::enums::Wrap_Mode::Mirror_Repeat:
        strcpy_s(wrap_t_text, 14, "Mirror_Repeat");
        break;
    case tilia::enums::Wrap_Mode::Clamp_Edge:
        strcpy_s(wrap_t_text, 14, "Clamp_Edge\0");
        break;
    }
    switch (cube_map.Get_Data().wrap_r)
    {
    case tilia::enums::Wrap_Mode::Repeat:
        strcpy_s(wrap_r_text, 14, "Repeat\0");
        break;
    case tilia::enums::Wrap_Mode::Mirror_Repeat:
        strcpy_s(wrap_r_text, 14, "Mirror_Repeat");
        break;
    case tilia::enums::Wrap_Mode::Clamp_Edge:
        strcpy_s(wrap_r_text, 14, "Clamp_Edge\0");
        break;
    }

    // Prints to console indented
    tilia::log::Log_Indent("Size", "%d", cube_map.Get_Data().size);
    tilia::log::Log_Indent("Format", "Pos_x: %s, Neg_x: %s, Pos_y: %s, Neg_y: %s, Pos_z: %s, Neg_z: %s", format_text[0], format_text[1], format_text[2], format_text[3], format_text[4], format_text[5]);
    tilia::log::Log_Indent("Filter", "Min Filter: %s, Mag Filter: %s", filter_min_text, filter_max_text);
    tilia::log::Log_Indent("Wrapping", "Wrap S: %s, Wrap T: %s, Wrap R: %s", wrap_s_text, wrap_t_text, wrap_r_text);

}
*/

tilia::render::Cube_Map::Cube_Map()
{
    m_texture_type = enums::Texture_Type::Cube_Map;
    Generate_Texture();
}



void tilia::render::Cube_Map::Set(const Cube_Map_Data& texture_def)
{
    constexpr const size_t cube_sides{ *enums::Misc::Cube_Sides };

    Unbind(true);

    // Binds texture
    Bind();

    // Copies passed Texture_Def
    m_cube_map_data = texture_def;

    int32_t nr_load_channels[cube_sides] { 0 };

    int32_t nr_channels[cube_sides] { 0 };

    for (size_t i = 0; i < cube_sides; i++)
    {
        switch (m_cube_map_data.sides[i].color_format)
        {
        case enums::Color_Format::Red8:
            nr_channels[i] = 1;
            break;
        case enums::Color_Format::RGB8:
            nr_channels[i] = 3;
            break;
        case enums::Color_Format::RGBA8:
            nr_channels[i] = 4;
            break;
        }

        // Copies texture_data or loads new data using file_path
        if (texture_def.sides[i].texture_data) {
            uint32_t byte_count{ static_cast<uint32_t>((powf(static_cast<float>(m_cube_map_data.size), 2.0f) * nr_channels[i])) };
            m_cube_map_data.sides[i].texture_data = std::make_unique<uint8_t[]>(static_cast<size_t>(byte_count));
            // Copies data
            std::copy(texture_def.sides[i].texture_data.get(), texture_def.sides[i].texture_data.get() + byte_count, m_cube_map_data.sides[i].texture_data.get());
            if (!m_cube_map_data.sides[i].texture_data);
        }
        else if (!texture_def.sides[i].texture_data) {
            stbi_set_flip_vertically_on_load(0);
            // Loads data
            m_cube_map_data.sides[i].texture_data.reset(stbi_load(texture_def.sides[i].file_path.c_str(), &m_cube_map_data.size, &m_cube_map_data.size, &nr_load_channels[i], 0));
            if (!m_cube_map_data.sides[i].texture_data);
        }

        if (m_cube_map_data.sides[i].color_format == enums::Color_Format::None)
            m_cube_map_data.sides[i].color_format = m_cube_map_data.sides[i].load_color_format;

        // Sets load_color_format
        switch (nr_load_channels[i])
        {
        case 1:
            m_cube_map_data.sides[i].load_color_format = enums::Color_Format::Red;
            break;
        case 3:
            m_cube_map_data.sides[i].load_color_format = enums::Color_Format::RGB;
            break;
        case 4:
            m_cube_map_data.sides[i].load_color_format = enums::Color_Format::RGBA;
            break;
        }

        if (m_cube_map_data.sides[i].load_color_format == enums::Color_Format::None)
            m_cube_map_data.sides[i].load_color_format = m_cube_map_data.sides[i].color_format;

        // Set unpack alignment
        if (m_cube_map_data.sides[i].load_color_format == enums::Color_Format::RGBA)
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
        GL_CALL(glTexImage2D(*enums::Cube_Map_Sides::Positive_X + static_cast<int32_t>(i), 0,
            *m_cube_map_data.sides[i].color_format, 
            m_cube_map_data.size, m_cube_map_data.size, 0, 
            *m_cube_map_data.sides[i].load_color_format, 
            GL_UNSIGNED_BYTE, 
            m_cube_map_data.sides[i].texture_data.get()));

    }
    
    // Unbinds texture
    Rebind();

    //log::Log(log::Type::INFO, "TEXTURE_CUBE_MAP", "Texture { ID: %u } data has been set", m_ID);

    //Print_Information(*this);

}

/**
 * Calls the overloaded version of Set_Texture that takes a cube map def with the file paths
 * set to the given paths.
 */
void tilia::render::Cube_Map::Set(const std::array<std::string, 6>& texture_paths)
{
    Cube_Map_Data def{};
    for (size_t i = 0; i < *enums::Misc::Cube_Sides; i++)
    {
        def.sides[i].file_path = texture_paths[i];
    }
    Set(def);
}

/**
 * Generates mipmaps for the openGL texture using glGenerateMipmap
 */
void tilia::render::Cube_Map::Generate_Mipmaps()
{
    Unbind(true);
    if (m_ID == 0) {
        return log::Log(log::Type::ERROR, "TEXTURE_CUBE_MAP", "Tried to generate mipmap levels for unloaded Texture");
        Rebind();
    }
    constexpr const size_t cube_sides{ *enums::Misc::Cube_Sides };
    for (size_t i = 0; i < cube_sides; i++)
    {
        if (!m_cube_map_data.sides[i].texture_data)
        {
            return log::Log(log::Type::ERROR, "TEXTURE_CUBE_MAP", "Texture { ID: %u } did not generate mipmap levels because there is no data", m_ID);
            Rebind();
        }
    }
    GL_CALL(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
    log::Log(log::Type::INFO, "TEXTURE_CUBE_MAP", "Mipmaps for texture { ID: %u } has been generated", m_ID);
    Rebind();
}

/**
 * Sets the filtering mode for the given size
 */
void tilia::render::Cube_Map::Set(const enums::Filter_Size& filter_size, const enums::Filter_Mode& filter_mode)
{
    Unbind(true);
    GL_CALL(glTexParameteri(*m_texture_type, *filter_size, *filter_mode));
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
void tilia::render::Cube_Map::Set(const enums::Wrap_Sides& wrap_side, const enums::Wrap_Mode& wrap_mode)
{
    Unbind(true);
    GL_CALL(glTexParameteri(*m_texture_type, *wrap_side, *wrap_mode));
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
