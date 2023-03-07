/**************************************************************************************************
 * @file   Callbacks.hpp
 * 
 * @brief  
 * 
 * @author Gustav Fagerlind
 * @date   06/03/2023
 *************************************************************************************************/

#ifndef TILIA_WINDOW_PROPERTIES_HPP
#define TILIA_WINDOW_PROPERTIES_HPP

// Standard
#include <cstdint>
#include <functional>
#include <tuple>

// Tilia
#include "Core/Values/Directories.hpp"
#include TILIA_CONSTANTS_INCLUDE

namespace tilia
{

	namespace utils
	{

		typedef struct GLFWwindow GLFWwindow; // GLFWwindow

	} // utils

	namespace callbacks
	{

		template<enums::Window_Callbacks Func_Type, typename Func_Parameters>
		struct Window_Func
		{
			using Parameters = Func_Parameters;
			using Signature = std::function<Parameters>;
			static constexpr auto Type{ Func_Type };
			Signature function{};
			Window_Func(Signature func) : function{ func } { }
		}; // Window_Func

		using Position = Window_Func<enums::Window_Callbacks::Position,
			void(utils::GLFWwindow*, std::int32_t, std::int32_t)>;

		using Size = Window_Func<enums::Window_Callbacks::Size, 
			void(utils::GLFWwindow*, std::int32_t, std::int32_t)>;

		using Close = Window_Func<enums::Window_Callbacks::Close, 
			void(utils::GLFWwindow*)>;

		using Refresh = Window_Func<enums::Window_Callbacks::Refresh, 
			void(utils::GLFWwindow*)>;

		using Focus = Window_Func<enums::Window_Callbacks::Focus, 
			void(utils::GLFWwindow*, std::int32_t)>;

		using Inconify = Window_Func<enums::Window_Callbacks::Iconify, 
			void(utils::GLFWwindow*, std::int32_t)>;

		using Maximize = Window_Func<enums::Window_Callbacks::Maximize, 
			void(utils::GLFWwindow*, std::int32_t)>;

		using Framebuffer_Size = Window_Func<enums::Window_Callbacks::Framebuffer_Size, 
			void(utils::GLFWwindow*, std::int32_t, std::int32_t)>;

		using Content_Scale = Window_Func<enums::Window_Callbacks::Content_Scale, 
			void(utils::GLFWwindow*, float, float)>;

	} // callbacks

	namespace properties
	{

		template<enums::Window_Properties Property_Type, typename... Property_Parameters>
		struct Window_Property
		{
			using Parameters = std::tuple<Property_Parameters...>;
			static constexpr auto Type{ Property_Type };
			Parameters parameters{};
			Window_Property(Property_Parameters&&... params)
				: parameters{ (std::forward<Property_Parameters>(params))... } { }
		};

	} // properties

} // tilia

#endif // TILIA_WINDOW_PROPERTIES_HPP
