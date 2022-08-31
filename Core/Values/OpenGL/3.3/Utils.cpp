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
uint32_t tilia::utils::Get_Max_Textures()
{
	int32_t amount{};
	GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &amount));
	return static_cast<uint32_t>(amount);
}

/**
 * Gets the amount of indecies needed for the given primitve
 */
uint32_t tilia::utils::Get_Primitive_Index_Count(const uint32_t& primitve)
{
	switch (primitve)
	{
	case GL_POINTS:
		return 1ui32;
	case GL_LINES:
	case GL_LINE_LOOP:
	case GL_LINE_STRIP:
		return 2ui32;
	case GL_TRIANGLES:
	case GL_TRIANGLE_STRIP:
	case GL_TRIANGLE_FAN:
		return 3ui32;
	default:
		return 1ui32;
	}
}

uint32_t tilia::utils::Get_Color_Format_Count(const uint32_t& color_format)
{
	switch (color_format)
	{
	case GL_RED:
	case GL_R8:
		return 1ui32;
	case GL_RGB:
	case GL_RGB8:
		return 3ui32;
	case GL_RGBA:
	case GL_RGBA8:
		return 4ui32;
	case GL_NONE:
	default:
		return 0ui32;
	}
}

const char* tilia::utils::Get_Cube_Map_Side(const uint32_t& cube_map_side)
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

tilia::enums::Data_Color_Format tilia::utils::Get_Data_Color_Format(const uint32_t& color_format_count)
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

tilia::enums::Color_Format tilia::utils::Get_Color_Format(const uint32_t& color_format_count)
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
