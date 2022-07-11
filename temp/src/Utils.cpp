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
#include "dependencies/glad/include/glad/glad.h"

// Headers
#include "headers/Utils.h"
#include "headers/Error_Handling.h"

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
constexpr uint32_t tilia::utils::Get_Primitive_Index_Count(const uint32_t& primitve)
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
