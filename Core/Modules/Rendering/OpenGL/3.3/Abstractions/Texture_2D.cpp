/*****************************************************************//**
 * @file   Texture.cpp
 * @brief  Defines all non-inline member functions and constructors/destructors of the @see @Texture_2D_Def struct 
 *		   and @see @Texture_2D class.
 * 
 * Dependencies:
 * @include "dependencies/glad/include/glad/glad.h"
 * @include "dependencies/stb_image/include/stb_image/stb_image.h"
 * 
 * Standard:
 * @include <stdexcept>
 * 
 * Headers:
 * @include "headers/Utils.h"
 * @include "headers/Texture.h"
 * @include "headers/Error_Handling.h"
 * @include "headers/Logging.h"
 * 
 * @author Gustav Fagerlind
 * @date   15/05/2022
 *********************************************************************/

// Dependencies
#include "vendor/glad/include/glad/glad.h"

// Standard
#include <stdexcept>

// Headers
#include "Core/Values/OpenGL/3.3/Utils.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Texture_2D.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.hpp"
#include "Core/Modules/Console/Logging.hpp"
#include "Core/Modules/File_System/Windows/File_System.hpp"
#include "Core/Modules/Exceptions/Tilia_Exception.hpp"

extern tilia::utils::File_System file_system;

/**
 * Checks a couple of enums pertaining to the color format, wrapping modes, 
 * and filtering modes and sets some C-string with the information. Then
 * prints it all using Log_Indent.
 */
void tilia::render::Texture_2D::Print_Information() const
{

	// Color format C-string
	char format_text[6]{};
	// Get color format string
	switch (m_texture_def.color_format)
	{
	case enums::Color_Format::Red8:
		strcpy_s(format_text, 6, "Red8\0");
		break;
	case enums::Color_Format::RGB8:
		strcpy_s(format_text, 6, "RGB8\0");
		break;
	case enums::Color_Format::RGBA8:
		strcpy_s(format_text, 6, "RGBA8");
		break;
	}
	// Filtering C-strings
	char filter_min_text[10]{};
	char filter_max_text[10]{};
	// Gets filtering mode strings
	switch (m_texture_def.filter_min)
	{
	case enums::Filter_Mode::Point:
		strcpy_s(filter_min_text, 10, "Point\0");
		break;
	case enums::Filter_Mode::Bilinear:
		strcpy_s(filter_min_text, 10, "Bilinear\0");
		break;
		//case Filter_Mode::Trilinear:
		//	strcpy_s(filter_min_text, 10, "Trilinear");
		//	break;
	}
	switch (m_texture_def.filter_mag)
	{
	case enums::Filter_Mode::Point:
		strcpy_s(filter_max_text, 10, "Point\0");
		break;
	case enums::Filter_Mode::Bilinear:
		strcpy_s(filter_max_text, 10, "Bilinear\0");
		break;
	}
	// Wrapping C-string
	char wrap_s_text[14]{};
	char wrap_t_text[14]{};
	// Gets wrapping mode strings
	switch (m_texture_def.wrap_s)
	{
	case enums::Wrap_Mode::Repeat:
		strcpy_s(wrap_s_text, 14, "Repeat\0");
		break;
	case enums::Wrap_Mode::Mirror_Repeat:
		strcpy_s(wrap_s_text, 14, "Mirror_Repeat");
		break;
	case enums::Wrap_Mode::Clamp_Edge:
		strcpy_s(wrap_s_text, 14, "Clamp_Edge\0");
		break;
	}
	switch (m_texture_def.wrap_t)
	{
	case enums::Wrap_Mode::Repeat:
		strcpy_s(wrap_t_text, 14, "Repeat\0");
		break;
	case enums::Wrap_Mode::Mirror_Repeat:
		strcpy_s(wrap_t_text, 14, "Mirror_Repeat");
		break;
	case enums::Wrap_Mode::Clamp_Edge:
		strcpy_s(wrap_t_text, 14, "Clamp_Edge\0");
		break;
	}

	// Prints to console indented
	//log::Log_Indent("Dimensions", "%dx%d", m_texture_def.width, m_texture_def.height);
	//log::Log_Indent("Format", "%s", format_text);
	//log::Log_Indent("Filter", "Min Filter: %s, Mag Filter: %s", filter_min_text, filter_max_text);
	//log::Log_Indent("Wrapping", "Wrap S: %s, Wrap T: %s", wrap_s_text, wrap_t_text);

}

/**
 * Calls Generate_Texture function
 */
tilia::render::Texture_2D::Texture_2D()
{
	m_texture_type = enums::Texture_Type::TwoD;
	Generate_Texture();
}

#include <iostream>

/**
 * Firstly it copies the data from the given Texture_Def to the member Texture_Def. It also uses std::make_unique to create a new pointer for 
 * texture_data of the member Texture_Def. If the passed Texture_Def does not hold any texture data then the texture_data 
 * of the memeber Texture_Def is loaded in using stbi_load using the passed Texture_Def's member file_path. After that is sets the load_color_format 
 * according to the amount of loaded channels Then it sets the unpack alignment according to the color format. Then it binds the texture and sets 
 * the filtering and wrapping options. After that it finally sets the pixel data of the openGL texture and then unbinds the texture. It also prints 
 * information about the texture
 */
void tilia::render::Texture_2D::Set_Texture(const Texture_2D_Def& texture_def)
{
	// Copies passed Texture_Def
	m_texture_def = texture_def;

	int32_t nr_load_channels{ 0 };

	int32_t nr_channels{ 0 };

	// Copies texture_data or loads new data using file_path
	if (texture_def.texture_data) {

		switch (m_texture_def.color_format)
		{
		case enums::Color_Format::Red8:
			nr_channels = 1;
			break;
		case enums::Color_Format::RGB8:
			nr_channels = 3;
			break;
		case enums::Color_Format::RGBA8:
			nr_channels = 4;
			break;
		default:
			utils::Tilia_Exception e{ LOCATION };
			e.Add_Message("Texture_2D { ID: %v } was given an invalid color format"
			"\n>>> Format: %v")
				(m_ID)(*m_texture_def.color_format);
			throw e;
		}

		uint32_t byte_count{ static_cast<uint32_t>((m_texture_def.width * m_texture_def.height * nr_channels)) };

		if (!byte_count) {
			utils::Tilia_Exception e{ LOCATION };
			e.Add_Message("Texture_2D { ID: %v } failed to copy texture data")(m_ID);
			throw e;
		}

		m_texture_def.texture_data = std::make_unique<uint8_t[]>(static_cast<size_t>(byte_count));
		// Copies data
		std::copy(texture_def.texture_data.get(), texture_def.texture_data.get() + byte_count, m_texture_def.texture_data.get());
		if (!m_texture_def.texture_data)
		{
			utils::Tilia_Exception e{ LOCATION };
			e.Add_Message("Texture_2D { ID: %v } failed to copy texture data")(m_ID);
			throw e;
		}
	}
	else if (!texture_def.texture_data) {
		// Loads data
		try
		{
			m_texture_def.texture_data.reset(file_system.Load_Image
			(texture_def.file_path.c_str(), m_texture_def.width, m_texture_def.height, nr_load_channels, 0, true));
		}
		catch (utils::Tilia_Exception& e)
		{
			e.Add_Message("Texture_2D { ID: %v } data not loaded properly")(m_ID);
			throw e;
		}
	}
	
	if (!m_texture_def.texture_data.get()) {
		utils::Tilia_Exception e{ LOCATION };
		e.Add_Message("Texture_2D { ID: %v } data not set properly")(m_ID);
		throw e;
	}

	// Sets load_color_format
	switch (nr_load_channels)
	{
	case 1:
		m_texture_def.load_color_format = enums::Data_Color_Format::Red;
		break;
	case 3:
		m_texture_def.load_color_format = enums::Data_Color_Format::RGB;
			break;
	case 4:
		m_texture_def.load_color_format = enums::Data_Color_Format::RGBA;
			break;
	default:
		//m_texture_def.load_color_format = m_texture_def.color_format;
		break;
	}

	if (m_texture_def.color_format == enums::Color_Format::None)
		//m_texture_def.color_format = m_texture_def.load_color_format;

	// Set unpack alignment
	if (m_texture_def.load_color_format == enums::Data_Color_Format::RGBA)
	{
		GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
	}
	else
	{
		GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	}

	Unbind(true);

	// Binds texture
	Bind();

	// Sets filtering and wrapping modes
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, *m_texture_def.filter_min));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, *m_texture_def.filter_mag));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, *m_texture_def.wrap_s));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, *m_texture_def.wrap_t));

	// Sets pixel data
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, 
		*m_texture_def.color_format, 
		m_texture_def.width, m_texture_def.height, 0, 
		*m_texture_def.load_color_format, 
		GL_UNSIGNED_BYTE, 
		m_texture_def.texture_data.get()));
	
	// Unbinds texture
	Rebind();

	//log::Log(log::Type::INFO, "TEXTURE_2D", "Texture { ID: %u } data has been set", m_ID);

	//Print_Information();

}

/**
 * Calls the overloaded version of Set_Texture that takes a texture def with the file path 
 * set to the given path.
 */
void tilia::render::Texture_2D::Set_Texture(const std::string& texture_path)
{
	if (texture_path == "") {
		utils::Tilia_Exception e{ LOCATION };
		e.Add_Message("Texture_2D { ID: %v } file path is invalid"
			"\n>>> Path: %v")
			(m_ID)(texture_path);
		throw e;
	}
		
	Texture_2D_Def def{};
	def.file_path = texture_path;
	return Set_Texture(def);
}

/**
 * Generates mipmaps for the openGL texture using glGenerateMipmap
 */
void tilia::render::Texture_2D::Generate_Mipmaps()
{
	Unbind(true);
	if (m_ID == 0) {
		utils::Tilia_Exception e{ LOCATION };
		e.Add_Message("Texture_2D is not generated properly");
		throw e;
		Rebind();
	}
	if (!m_texture_def.texture_data)
	{
		utils::Tilia_Exception e{ LOCATION };
		e.Add_Message("Texture_2D { ID: %v } failed to generate mipmaps because there is no data")(m_ID);
		throw e;
		Rebind();
	}
	GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
	//log::Log(log::Type::INFO, "TEXTURE_2D", "Mipmaps for texture { ID: %u } has been generated", m_ID);
	Rebind();
}

/**
 * Sets the filtering mode for the given size
 */
void tilia::render::Texture_2D::Set(const enums::Filter_Size& filter_size, const enums::Filter_Mode& filter_mode)
{
	switch (filter_size)
	{
	case enums::Filter_Size::Magnify:
		m_texture_def.filter_mag = filter_mode;
		break;
	case enums::Filter_Size::Minify:
		m_texture_def.filter_min = filter_mode;
		break;
	}
	Unbind(true);
	GL_CALL(glTexParameteri(*m_texture_type, *filter_size, *filter_mode));
	Rebind();
}

/**
 * Sets the wrapping mode for the given side
 */
void tilia::render::Texture_2D::Set(const enums::Wrap_Sides& wrap_side, const enums::Wrap_Mode& wrap_mode)
{
	switch (wrap_side)
	{
	case enums::Wrap_Sides::S:
		m_texture_def.wrap_s = wrap_mode;
		break;
	case enums::Wrap_Sides::T:
		m_texture_def.wrap_t = wrap_mode;
		break;
	default:
		utils::Tilia_Exception e{ LOCATION };
		e.Add_Message("Wrap side: %v is not allowed for Texture_2D")(*wrap_side);
		throw e;
	}
	Unbind(true);
	GL_CALL(glTexParameteri(*m_texture_type, *wrap_side, *wrap_mode));
	Rebind();
}
