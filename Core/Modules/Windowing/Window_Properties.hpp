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
#include <string>

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

		using Set_Should_Close_Parameters = std::tuple<const bool&>;
		using Get_Should_Close_Parameters = std::tuple<bool>;

		using Set_Size_Parameters = std::tuple<const int32_t&, const int32_t&>;
		using Get_Size_Parameters = std::tuple<int32_t&, int32_t&>;

		using Get_Frame_Size_Parameters = std::tuple<int32_t&, int32_t&, int32_t&, int32_t&>;

		using Get_Framebuffer_Size_Parameters = std::tuple<int32_t&, int32_t&>;

		using Get_Content_Scale_Parameters = std::tuple<float&, float&>;

		using Set_Size_Limits_Parameters = std::tuple<const int32_t&, const int32_t&, 
			const int32_t&, const int32_t&>;

		using Set_Size_Parameters = std::tuple<const int32_t&, const int32_t&>;

		using Set_Position_Parameters = std::tuple<const int32_t&, const int32_t&>;
		using Get_Position_Parameters = std::tuple<int32_t&, int32_t&>;

		using Set_Title_Parameters = std::tuple<const std::string&>;

		using Set_Icon_Parameters = std::tuple<nullptr_t>;

		using Set_Monitor_Parameters = std::tuple<nullptr_t>;

		using Set_Iconify_Parameters = std::tuple<const bool&>;
		using Get_Iconify_Parameters = std::tuple<bool&>;

		using Set_Maximize_Parameters = std::tuple<const bool&>;
		using Get_Maximize_Parameters = std::tuple<bool&>;

		using Set_Visibility_Parameters = std::tuple<const bool&>;
		using Get_Visibility_Parameters = std::tuple<bool&>;

		using Set_Focus_Parameters = std::tuple<void>;
		using Get_Focus_Parameters = std::tuple<bool&>;



	} // properties

} // tilia

#endif // TILIA_WINDOW_PROPERTIES_HPP
