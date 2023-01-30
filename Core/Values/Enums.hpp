/**************************************************************************************************
 * @file   Enums.hpp
 * @brief  Differnent constant values which are used throughout the Tilia project
 *
 * @author Gustav Fagerlind
 * @date   07/01/2023
 *************************************************************************************************/

#ifndef TILIA_ENUMS_HPP
#define TILIA_ENUMS_HPP

 // Standard
#include <cstdint>
#include <type_traits>

// Tilia
#include "Core/Values/Directories.hpp"

#if defined(TILIA_OPENGL_3_3_ENUMS) && !defined(TILIA_OPENGL_3_3_ENUMS_HPP)

#include TILIA_OPENGL_3_3_ENUMS_HPP_INCLUDE

#endif // TILIA_OPENGL_3_3_ENUMS

namespace tilia
{
    namespace enums
    {

#ifndef TILIA_GET_ENUM_VALUE
#define TILIA_GET_ENUM_VALUE

		/**
		 * @brief Can take any enum value as parameter and will return (if any) the underlying value.
		 *
		 * @param e - The enum value
		 *
		 * @return The underlying value of the enum value
		 */
		template<typename E>
		constexpr std::underlying_type_t<E> Get_Enum_Value(const E& e) noexcept {
			return static_cast<std::underlying_type_t<E>>(e);
		}

		/**
		 * @brief Can take any enum value as parameter and will return (if any) the underlying value.
		 *
		 * @param e - The enum value
		 *
		 * @return The underlying value of the enum value
		 */
		template<typename E>
		constexpr auto operator*(const E& e) {
			return Get_Enum_Value(e);
		}

#endif // TILIA_GET_ENUM_VALUE

    }// enums
} // tilia

/**
 * @brief Whether or not to include unit tests
 */
#define TILIA_UNIT_TESTS 1

#endif // TILIA_ENUMS_HPP
