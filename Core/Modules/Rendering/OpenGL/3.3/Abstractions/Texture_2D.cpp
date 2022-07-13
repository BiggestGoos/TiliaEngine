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
#include "dependencies/glad/include/glad/glad.h"
#include "dependencies/stb_image/include/stb_image/stb_image.h"

// Standard
#include <stdexcept>

// Headers
#include "Core/Values/OpenGL/3.3/Utils.h"
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Texture_2D.h"
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.h"
#include "Core/Modules/Console/Logging.h"
#include "Core/Modules/File_System/Windows/File_System.h"
#include "Core/Modules/Exceptions/Tilia_Exception.h"

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
	}

	// Copies texture_data or loads new data using file_path
	if (texture_def.texture_data) {
		uint32_t byte_count{ static_cast<uint32_t>((m_texture_def.width * m_texture_def.height * nr_channels)) };
		m_texture_def.texture_data = std::make_unique<uint8_t[]>(static_cast<size_t>(byte_count));
		// Copies data
		std::copy(texture_def.texture_data.get(), texture_def.texture_data.get() + byte_count, m_texture_def.texture_data.get());
		if (!m_texture_def.texture_data)
			throw utils::Tilia_Exception{"", __LINE__, __FILE__};
	}
	else if (!texture_def.texture_data) {
		stbi_set_flip_vertically_on_load(1);
		// Loads data
		m_texture_def.texture_data.reset(utils::file_system.Load_Image
		(texture_def.file_path.c_str(), m_texture_def.width, m_texture_def.height, nr_load_channels, 0, true));
	}

	if (m_texture_def.color_format == enums::Color_Format::None)
		m_texture_def.color_format = m_texture_def.load_color_format;

	// Sets load_color_format
	switch (nr_load_channels)
	{
	case 1:
		m_texture_def.load_color_format = enums::Color_Format::Red;
		break;
	case 3:
		m_texture_def.load_color_format = enums::Color_Format::RGB;
			break;
	case 4:
		m_texture_def.load_color_format = enums::Color_Format::RGBA;
			break;
	}

	if (m_texture_def.load_color_format == enums::Color_Format::None)
		m_texture_def.load_color_format = m_texture_def.color_format;

	// Set unpack alignment
	if (m_texture_def.load_color_format == enums::Color_Format::RGBA)
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

	log::Log(log::Type::INFO, "TEXTURE_2D", "Texture { ID: %u } data has been set", m_ID);

	Print_Information();

	return 0;

}

/**
 * Calls the overloaded version of Set_Texture that takes a texture def with the file path 
 * set to the given path.
 */
void tilia::render::Texture_2D::Set_Texture(const std::string& texture_path)
{
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
		return log::Log(log::Type::ERROR, "TEXTURE_2D", "Tried to generate mipmap levels for unloaded Texture");
		Rebind();
	}
	if (!m_texture_def.texture_data)
	{
		return log::Log(log::Type::ERROR, "TEXTURE_2D", "Texture { ID: %u } did not generate mipmap levels because there is no data", m_ID);
		Rebind();
	}
	GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
	log::Log(log::Type::INFO, "TEXTURE_2D", "Mipmaps for texture { ID: %u } has been generated", m_ID);
	Rebind();
}

/**
 * Sets the filtering mode for the given size
 */
void tilia::render::Texture_2D::Set_Parameter(const enums::Filter_Size& filter_size, const enums::Filter_Mode& filter_mode)
{
	Unbind(true);
	GL_CALL(glTexParameteri(*m_texture_type, *filter_size, *filter_mode));
	Rebind();
	switch (filter_size)
	{
	case enums::Filter_Size::Magnify:
		m_texture_def.filter_mag = filter_mode;
		break;
	case enums::Filter_Size::Minify:
		m_texture_def.filter_min = filter_mode;
		break;
	}
}

/**
 * Sets the wrapping mode for the given side
 */
void tilia::render::Texture_2D::Set_Parameter(const enums::Wrap_Sides& wrap_side, const enums::Wrap_Mode& wrap_mode)
{
	Unbind(true);
	GL_CALL(glTexParameteri(*m_texture_type, *wrap_side, *wrap_mode));
	Rebind();
	switch (wrap_side)
	{
	case enums::Wrap_Sides::S:
		m_texture_def.wrap_s = wrap_mode;
		break;
	case enums::Wrap_Sides::T:
		m_texture_def.wrap_t = wrap_mode;
		break;
	}
}
