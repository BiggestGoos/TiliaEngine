/*****************************************************************//**
 * @file   Utils.h
 * @brief  Contains utility functions and variables used for various things.
 * 
 * @define TILIA_UTILS_H
 * 
 * Standard:
 * @include <cstdint>
 * 
 * Headers:
 * @include "headers/Enums.h"
 * 
 * @author Gustav Fagerlind
 * @date   15/05/2022
 *********************************************************************/

#ifndef TILIA_UTILS_H
#define TILIA_UTILS_H

// Standard
#include <cstdint>

// Headers
#include "Core/Values/OpenGL/3_3/Enums.hpp"

namespace tilia {

	namespace utils {

		/**
		 * @brief A function which gets the max amount of textures the platform supports
		 * 
		 * @return The max amount of textures supported as a 32-bit integer
		 */
		std::uint32_t Get_Max_Textures();

		/**
		 * @brief Gets the smalles amount of indices needed for a primitve
		 * 
		 * @return The amount of indices needed for primitve
		 */
		std::uint32_t Get_Primitive_Index_Count(const std::uint32_t& primitve);

		std::uint32_t Get_Color_Format_Count(const std::uint32_t& color_format);

		const char* Get_Cube_Map_Side_String(const std::uint32_t& cube_map_side);

		enums::Data_Color_Format Get_Data_Color_Format(const std::uint32_t& color_format_count);
		enums::Color_Format Get_Color_Format(const std::uint32_t& color_format_count);

		std::size_t Get_Shader_Type_Index(const enums::Shader_Type& type);
		enums::Shader_Type Get_Index_Shader_Type(const std::size_t index);

		const char* Get_Shader_Type_String(const enums::Shader_Type& type);

		std::size_t Get_GLSL_Scalar_Size(const enums::GLSL_Scalar_Type& scalar);
		std::size_t Get_GLSL_Container_Alignment(const enums::GLSL_Container_Type& container);

	}

}

#endif