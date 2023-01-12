/*****************************************************************//**
 * @file   Texture.cpp
 * @brief  Defines all non-inline member functions and constructors/destructors of the @Texture class
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
 *********************************************************************/

// Dependencies
#include "vendor/glad/include/glad/glad.h"

// Standard
#include <string.h>

// Headers
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Texture_files/Texture.hpp"
#include "Core/Values/OpenGL/3_3/Utils.hpp"
#include "Core/Modules/Console/Logging.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.hpp"
#include "Core/Modules/Exceptions/Tilia_Exception.hpp"

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
		utils::Tilia_Exception e{ LOCATION };
		e.Add_Message("Texture { ID: %v } type is undefined"
			"\n>>> Type: %v")
			(m_ID)(*m_texture_type);
		throw e;
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
		GL_CALL(glGenTextures(1, &m_ID));
	}
	catch (utils::Tilia_Exception& e)
	{

		e.Add_Message("%v { ID: %v } failed to be generated")
			(Get_Type_String())(m_ID);

		// Possibly forward e to someplace else and then throw

	}
	//log::Log(log::Type::SUCCESS, Get_Type_String().c_str(), "Texture { ID: %u } was successfuly created", m_ID);
}

/**
 * Deletes the texture from openGL and frees m_texture_def.texture_data
 */
tilia::gfx::Texture::~Texture()
{
	try
	{
		GL_CALL(glDeleteTextures(1, &m_ID));
	}
	catch (utils::Tilia_Exception& e)
	{

		e.Add_Message("%v { ID: %v } failed to be destroyed")
			(Get_Type_String())(m_ID);

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
		utils::Tilia_Exception e{ LOCATION };
		e.Add_Message("Texture { Slot: %v } is out of the texture slot range { Min: 0, Max: %v }")
			(slot)(utils::Get_Max_Textures() - 1);
		throw e;
	}

	// Sets slot and binds texture
	GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
	GL_CALL(glBindTexture(*m_texture_type, m_ID));

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
	GL_CALL(glBindTexture(*m_texture_type, 0));

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
	GL_CALL(glBindTexture(*texture_type, 0));

	s_bound_ID[texture_type] = 0;

}

/**
 * Binds the stored s_previous_ID to the texture type, m_texture_type.
 */
void tilia::gfx::Texture::Rebind() const
{

	GL_CALL(glBindTexture(*m_texture_type, s_previous_ID[m_texture_type]));

}

/**
 * Static version of rebind.
 */
void tilia::gfx::Texture::Rebind(const enums::Texture_Type& texture_type)
{

	GL_CALL(glBindTexture(*texture_type, s_previous_ID[texture_type]));

}
