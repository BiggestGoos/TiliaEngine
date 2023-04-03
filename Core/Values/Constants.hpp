/**************************************************************************************************
 * @file   Constants.hpp
 * @brief  Different constant values which are used throughout the Tilia project.
 *
 * @author Gustav Fagerlind
 * @date   07/01/2023
 *************************************************************************************************/

#ifndef TILIA_CONSTANTS_HPP
#define TILIA_CONSTANTS_HPP

 // Standard
#include <cstdint>
#include <type_traits>

// Tilia
#include "Core/Values/Directories.hpp"

/**
 * @brief If 'TILIA_INCLUDE_OPENGL_3_3_ENUMS' is defined then we include the openGL 3.3 Tilia
 * constants.
 */
#if defined(TILIA_INCLUDE_OPENGL_3_3_CONSTANTS) && !defined(TILIA_OPENGL_3_3_CONSTANTS_HPP)

#include TILIA_OPENGL_3_3_CONSTANTS_INCLUDE

#endif // TILIA_OPENGL_3_3_ENUMS

namespace tilia
{
    namespace enums
    {

		// Limiting constants of Tilia batch.
		enum class Batch_Limits {
			Max_Triangles = 16384,		        // The max amount of triangles per batch.
			Max_Vertices  = Max_Triangles * 3,  // The max amount of vertices per batch.
			Max_Indices   = Max_Vertices * 2    // The max amount of indices per batch.
		}; // Batch_Limits

		// Different constants for features of different geometrical shapes.
		enum class Geometry_Features {
			Cube_Faces      = 6, // The amount of faces a cube has.
			Point_Points    = 1, // The amount of points in a point.
			Line_Points     = 2, // The amount of points in a line.
			Triangle_Points = 3, // The amount of points in a triangle.
			Quad_Points     = 4  // The amount of points in a quad.
		}; // Geometry_Features

#ifndef TILIA_GET_ENUM_VALUE
#define TILIA_GET_ENUM_VALUE

		/**
		 * @brief Can take any enum value as parameter and will return (if any) the underlying
		 * value.
		 *
		 * @param e - The enum value
		 *
		 * @return The underlying value of the enum value
		 */
		template<typename E>
		constexpr auto Get_Enum_Value(const E& e) noexcept {
			return static_cast<std::underlying_type_t<E>>(e);
		}

		/**
		 * @brief Can take any enum value as parameter and will return (if any) the underlying
		 * value.
		 *
		 * @param e - The enum value
		 *
		 * @return The underlying value of the enum value
		 */
		template<typename E>
		constexpr auto operator*(const E& e) noexcept {
			return Get_Enum_Value(e);
		}

#endif // TILIA_GET_ENUM_VALUE

    }// enums

#define TILIA_GET_UNDERLYING_ENUM_VALUE \
	template<typename E> \
	constexpr auto Get_Enum_Value(const E& e) noexcept { \
		return static_cast<std::underlying_type_t<E>>(e); \
	} \
	template<typename E> \
	constexpr auto operator*(const E& e) noexcept { \
		return Get_Enum_Value(e); \
	} \

	namespace windowing::enums
	{
		TILIA_GET_UNDERLYING_ENUM_VALUE
		namespace framebuffer
		{
			TILIA_GET_UNDERLYING_ENUM_VALUE
		} // framebuffer
		namespace context
		{
			TILIA_GET_UNDERLYING_ENUM_VALUE
		} // context
	} // windowing::enums

	namespace monitoring::enums
	{
		TILIA_GET_UNDERLYING_ENUM_VALUE
	} // monitoring::enums

	constexpr std::int32_t DO_NOT_CARE{ -1 };

} // tilia

/**
 * @brief Whether or not to include unit tests
 */
#define TILIA_UNIT_TESTS 0

#endif // TILIA_CONSTANTS_HPP
