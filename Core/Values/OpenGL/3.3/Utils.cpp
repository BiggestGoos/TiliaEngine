/*****************************************************************//**
 * @file   Utils.cpp
 * @brief  Defines the functions declared in the header @include "headers/Utils.h"
 * 
 * Dependencies:
 * @include "dependencies/glad/include/glad/glad.h"
 * 
 * Headers:
 * @include "headers/Utils.h"
 * @include "headers/Error_Handling.h"
 * 
 * @author Gustav Fagerlind
 * @date   15/05/2022
 *********************************************************************/

// Dependencies
#include "vendor/glad/include/glad/glad.h"

// Headers
#include "Core/Values/OpenGL/3.3/Utils.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.hpp"

/**
 * Gets the max amount of texture units supported and then returns them to the caller
 */
std::uint32_t tilia::utils::Get_Max_Textures()
{
	std::int32_t amount{};
	GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &amount));
	return static_cast<std::uint32_t>(amount);
}

/**
 * Gets the amount of indecies needed for the given primitve
 */
std::uint32_t tilia::utils::Get_Primitive_Index_Count(const std::uint32_t& primitve)
{
	switch (primitve)
	{
	case GL_POINTS:
		return 1;
	case GL_LINES:
	case GL_LINE_LOOP:
	case GL_LINE_STRIP:
		return 2;
	case GL_TRIANGLES:
	case GL_TRIANGLE_STRIP:
	case GL_TRIANGLE_FAN:
		return 3;
	default:
		return 1;
	}
}

std::uint32_t tilia::utils::Get_Color_Format_Count(const std::uint32_t& color_format)
{
	switch (color_format)
	{
	case GL_RED:
	case GL_R8:
		return 1;
	case GL_RGB:
	case GL_RGB8:
		return 3;
	case GL_RGBA:
	case GL_RGBA8:
		return 4;
	case GL_NONE:
	default:
		return 0;
	}
}

const char* tilia::utils::Get_Cube_Map_Side_String(const std::uint32_t& cube_map_side)
{
	switch (cube_map_side)
	{
	case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
		return "Pos X";
	case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
		return "Neg X";
	case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
		return "Pos Y";
	case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
		return "Neg Y";
	case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
		return "Pos Z";
	case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
		return "Neg Z";
	default:
		return "Error side";
	}
}

tilia::enums::Data_Color_Format tilia::utils::Get_Data_Color_Format(const std::uint32_t& color_format_count)
{
	switch (color_format_count)
	{
	case 1:
		return enums::Data_Color_Format::Red;
	case 3:
		return enums::Data_Color_Format::RGB;
	case 4:
		return enums::Data_Color_Format::RGBA;
	default:
		return enums::Data_Color_Format::None;
	}
}

tilia::enums::Color_Format tilia::utils::Get_Color_Format(const std::uint32_t& color_format_count)
{
	switch (color_format_count)
	{
	case 1:
		return enums::Color_Format::Red8;
	case 3:
		return enums::Color_Format::RGB8;
	case 4:
		return enums::Color_Format::RGBA8;
	default:
		return enums::Color_Format::None;
	}
}

std::size_t tilia::utils::Get_Shader_Type_Index(const enums::Shader_Type& type)
{
	switch (type)
	{
	case enums::Shader_Type::Vertex:
		return 0;
	case enums::Shader_Type::Fragment:
		return 1;
	case enums::Shader_Type::Geometry:
		return 2;
	default:
		return 3;
	}
}

tilia::enums::Shader_Type tilia::utils::Get_Index_Shader_Type(const std::size_t index)
{
	switch (index)
	{
	case 0:
		return enums::Shader_Type::Vertex;
	case 1:
		return enums::Shader_Type::Fragment;
	case 2:
		return enums::Shader_Type::Geometry;
	default:
		return enums::Shader_Type();
	}
}

const char* tilia::utils::Get_Shader_Type_String(const enums::Shader_Type& type)
{
	switch (type)
	{
	case enums::Shader_Type::Vertex:
		return "Vertex";
	case enums::Shader_Type::Fragment:
		return "Fragment";
	case enums::Shader_Type::Geometry:
		return "Geometry";
	default:
		return "Error type";
	}
}

std::size_t tilia::utils::Get_GLSL_Scalar_Size(const enums::GLSL_Scalar_Type& scalar)
{

	switch (scalar)
	{
	case enums::GLSL_Scalar_Type::Bool:
		return sizeof(GLboolean);
	case enums::GLSL_Scalar_Type::Int:
		return sizeof(GLint);
	case enums::GLSL_Scalar_Type::Uint:
		return sizeof(GLuint);
	case enums::GLSL_Scalar_Type::Float:
		return sizeof(GLfloat);
	default:
		return 0;
	}

}
