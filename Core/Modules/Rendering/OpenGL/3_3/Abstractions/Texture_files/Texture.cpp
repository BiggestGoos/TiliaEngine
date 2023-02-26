/**************************************************************************************************
 * @file   Texture.cpp
 * @brief  Defines all non-inline member functions and constructors/destructors of the @Texture
 * class
 * 
 * Dependencies:
 * @include "dependencies/glad/include/glad/glad.h"
 * 
 * Standard:
 * @include <string.h>
 * 
 * Headers:
 * @include "headers/Texture.h"
 * @include "headers/Utils.h"
 * @include "headers/Logging.h"	
 * @include "headers/Error_Handling.h"	
 * 
 * @author Gustav Fagerlind
 * @date   02/06/2022
 *************************************************************************************************/

// Dependencies
#include "vendor/glad/KHR_Debug_openGL_3_3/include/glad/glad.h"

// Standard
#include <string.h>

// Headers
#include "Texture.hpp"
#include "Core/Values/Directories.hpp"
#include TILIA_OPENGL_3_3_ERROR_HANDLING_INCLUDE
#include TILIA_OPENGL_3_3_UTILS_INCLUDE
#include TILIA_LOGGING_INCLUDE
#include TILIA_TILIA_EXCEPTION_INCLUDE

// Initialize static member which holds the bound textures ids
std::unordered_map<tilia::enums::Texture_Type, uint32_t> tilia::gfx::Texture::s_bound_ID{};
// Initialize static member which holds the previously bound textures ids
std::unordered_map<tilia::enums::Texture_Type, uint32_t> tilia::gfx::Texture::s_previous_ID{};

/**
 * @brief Returns the type of texture as a string
 * 
 * @return The type of texture as a string
 */
std::string tilia::gfx::Texture::Get_Type_String() const
{
	
	char texture_type_string[19]{ "TEXTURE_" };
	// Gets texture type as string
	switch (m_texture_type)
	{
	case tilia::enums::Texture_Type::OneD:
		strcat_s(texture_type_string, 19, "1D\0");
		break;
	case tilia::enums::Texture_Type::TwoD:
		strcat_s(texture_type_string, 19, "2D\0");
		break;
	case tilia::enums::Texture_Type::Cube_Map:
		strcat_s(texture_type_string, 19, "CUBE_MAP");
		break;
	default:
		throw utils::Tilia_Exception{ utils::Exception_Data{ TILIA_LOCATION } 
			<< "Texture { ID: " << m_ID << " } type is undefined"
			<< "\n>>> Type: " << *m_texture_type };
	}

	return texture_type_string;

}

/**
 * Generates a new texture and sets the id of the Texture using glad functions
 */
void tilia::gfx::Texture::Generate_Texture()
{
	try
	{
		GL_CALL_(glGenTextures(1, &m_ID));
	}
	catch (utils::Tilia_Exception& t_e)
	{

		t_e.Add_Message(TILIA_LOCATION) 
			<< Get_Type_String() << " { ID: " << m_ID << " } failed to be generated";

		// Possibly forward e to someplace else and then throw

	}
	//log::Log(log::Type::SUCCESS, Get_Type_String().c_str(), "Texture { ID: %u } was successfuly
	//created", m_ID);
}

/**
 * Deletes the texture from openGL and frees m_texture_def.texture_data
 */
tilia::gfx::Texture::~Texture()
{
	try
	{
		GL_CALL_(glDeleteTextures(1, &m_ID));
	}
	catch (utils::Tilia_Exception& t_e)
	{

		t_e.Add_Message(TILIA_LOCATION)
			<< Get_Type_String() << " { ID: " << m_ID << " } failed to be destroyed";

		// Possibly forward e to someplace else and then throw

	}
}

/**
 * Binds the openGL texture with m_ID to the given slot. If slot is outside 
 * of range, prints errors. Also sets s_bound_ID of m_texture_type to m_ID.
 */
void tilia::gfx::Texture::Bind(const uint32_t& slot) const
{

	// Checks if slot is in range and prints errors if not
	if (slot > utils::Get_Max_Textures() - 1) {
		throw utils::Tilia_Exception{ utils::Exception_Data{ TILIA_LOCATION } 
			<< "Texture { Slot: " << slot << " } is out of the texture slot range { Min: 0, Max: " 
			<< utils::Get_Max_Textures() - 1 << " }" };
	}

	// Sets slot and binds texture
	GL_CALL_(glActiveTexture(GL_TEXTURE0 + slot));
	GL_CALL_(glBindTexture(*m_texture_type, m_ID));

	s_bound_ID[m_texture_type] = m_ID;

}

/**
 * Unbinds the currently bound texture. First checks if you want to save the previously 
 * bound texture id. If so then gets the currently bound texture id and stores it in 
 * s_previous_ID. Otherwise it sets s_previous_ID to 0. After that it binds the texture
 * of the type m_texture_type to 0, effectively unbinding it. Also sets s_bound_ID of 
 * m_texture_type to 0.
 */
void tilia::gfx::Texture::Unbind(const bool& save_id) const
{

	// Checks save_id
	if (save_id)
	{
		// Stores previous id
		s_previous_ID[m_texture_type] = s_bound_ID[m_texture_type];
	}
	else 
	{
		s_previous_ID[m_texture_type] = 0;
	}

	// Binds texture of type m_texture_type to 0
	GL_CALL_(glBindTexture(*m_texture_type, 0));

	s_bound_ID[m_texture_type] = 0;

}

/**
 * Static version of unbind. Takes in a texture_type instead of using member.
 */
void tilia::gfx::Texture::Unbind(const enums::Texture_Type& texture_type, const bool& save_id)
{

	// Checks save_id
	if (save_id)
	{
		// Stores previous id
		s_previous_ID[texture_type] = s_bound_ID[texture_type];
	}
	else
	{
		s_previous_ID[texture_type] = 0;
	}

	// Binds texture of type texture_type to 0
	GL_CALL_(glBindTexture(*texture_type, 0));

	s_bound_ID[texture_type] = 0;

}

/**
 * Binds the stored s_previous_ID to the texture type, m_texture_type.
 */
void tilia::gfx::Texture::Rebind() const
{

	GL_CALL_(glBindTexture(*m_texture_type, s_previous_ID[m_texture_type]));

}

/**
 * Static version of rebind.
 */
void tilia::gfx::Texture::Rebind(const enums::Texture_Type& texture_type)
{

	GL_CALL_(glBindTexture(*texture_type, s_previous_ID[texture_type]));

}
