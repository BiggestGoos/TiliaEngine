/**************************************************************************************************
 * @file   Callbacks.hpp
 * 
 * @brief  
 * 
 * @author Gustav Fagerlind
 * @date   06/03/2023
 *************************************************************************************************/

#ifndef TILIA_WINDOWING_CALLBACKS_HPP
#define TILIA_WINDOWING_CALLBACKS_HPP

// Standard
#include <cstdint>
#include <functional>

// Tilia
#include "Core/Values/Directories.hpp"
#include TILIA_CONSTANTS_INCLUDE

namespace tilia
{

	namespace utils
	{

		typedef struct GLFWwindow GLFWwindow; // GLFWwindow

		template<enums::Window_Callbacks Func_Type, typename Func_Parameters>
		struct Window_Func
		{
			using Parameters = Func_Parameters;
			using Signature = std::function<Parameters>;
			static constexpr auto Type{ Func_Type };
			Signature function{};
			Window_Func(Signature func) : function{ func } { }
		}; // Window_Func

		using Position_Func = Window_Func<enums::Window_Callbacks::Position,
			void(utils::GLFWwindow*, std::int32_t, std::int32_t)>;

		using Size_Func = Window_Func<enums::Window_Callbacks::Size, 
			void(utils::GLFWwindow*, std::int32_t, std::int32_t)>;

		using Close_Func = Window_Func<enums::Window_Callbacks::Close, 
			void(utils::GLFWwindow*)>;

		using Refresh_Func = Window_Func<enums::Window_Callbacks::Refresh, 
			void(utils::GLFWwindow*)>;

		using Focus_Func = Window_Func<enums::Window_Callbacks::Focus, 
			void(utils::GLFWwindow*, std::int32_t)>;

		using Inconify_Func = Window_Func<enums::Window_Callbacks::Iconify, 
			void(utils::GLFWwindow*, std::int32_t)>;

		using Maximize_Func = Window_Func<enums::Window_Callbacks::Maximize, 
			void(utils::GLFWwindow*, std::int32_t)>;

		using Framebuffer_Size_Func = Window_Func<enums::Window_Callbacks::Framebuffer_Size, 
			void(utils::GLFWwindow*, std::int32_t, std::int32_t)>;

		using Content_Scale_Func = Window_Func<enums::Window_Callbacks::Content_Scale, 
			void(utils::GLFWwindow*, float, float)>;

	} // utils

} // tilia

#endif // TILIA_WINDOWING_CALLBACKS_HPP
