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

		static constexpr bool Setter{ true };
		static constexpr bool Getter{ false };

		template<bool Property_Access_Type, enums::Window_Properties Property_Type, typename T, typename U>
		struct Window_Property {
		};

		template<bool Property_Access_Type, enums::Window_Properties Property_Type, typename... Set_Parameters, typename... Get_Parameters>
		struct Window_Property<Property_Access_Type, Property_Type, std::tuple<Set_Parameters...>, std::tuple<Get_Parameters...>>
		{
			using Setter_Parameters  = std::tuple<Set_Parameters...>;
			using Getter_Parameters  = std::tuple<Get_Parameters...>;
			using First_Setter_Param = std::tuple_element_t<0, Setter_Parameters>;
			using First_Getter_Param = std::tuple_element_t<0, Getter_Parameters>;

			static constexpr auto Type{ Property_Type };
			static constexpr auto Access_Type{ Property_Access_Type };

			Setter_Parameters set_parameters;
			Getter_Parameters get_parameters;

			template<typename = std::enable_if_t<Access_Type == Setter>>
			Window_Property(Set_Parameters&&... params)
				: set_parameters{ (std::forward<Set_Parameters>(params))... },
				get_parameters{ Get_Temp_Ref<Get_Parameters...>() } {}

			template<typename = std::enable_if_t<Access_Type == Getter>>
			Window_Property(Get_Parameters&&... params)
				: get_parameters{ (std::forward<Get_Parameters>(params))... },
				set_parameters{ Get_Temp_Ref<Set_Parameters...>() } {}

		protected:
			template<typename... Types>
			auto Get_Temp_Ref()
			{
				std::tuple<typename std::remove_reference<Types>::type...> temp{};
				return std::apply([](auto&... args)
					{
						return std::tuple<std::add_lvalue_reference_t<decltype(args)>...>(args...);
					}, temp);
			}
		};

		using Set_Should_Close_Parameters = std::tuple<const bool&>;
		using Get_Should_Close_Parameters = std::tuple<bool&>;

		template<bool Property_Access_Type>
		using Should_Close = Window_Property<Property_Access_Type, enums::Window_Properties::Should_Close, std::tuple<const bool&>, std::tuple<bool&>>;

		using Set_Should_Close = Should_Close<Setter>;
		using Get_Should_Close = Should_Close<Getter>;

		template<bool Property_Access_Type>
		using Size = Window_Property<Property_Access_Type, enums::Window_Properties::Size, std::tuple<const int32_t&, const int32_t&>, std::tuple<int32_t&, int32_t&>>;

		using Set_Size= Size<Setter>;
		using Get_Size= Size<Getter>;

		template<bool Property_Access_Type>
		using Frame_Size = Window_Property<Property_Access_Type, enums::Window_Properties::Frame_Size, std::tuple<nullptr_t>, std::tuple<int32_t&, int32_t&, int32_t&, int32_t&>>;

		using Get_Frame_Size= Frame_Size<Getter>;
		using Set_Frame_Size= Frame_Size<Setter>;

		template<bool Property_Access_Type>
		using Framebuffer_Size = Window_Property<Property_Access_Type, enums::Window_Properties::Framebuffer_Size, std::tuple<nullptr_t>, std::tuple<int32_t&, int32_t&>>;

		using Set_Framebuffer_Size = Framebuffer_Size<Setter>;
		using Get_Framebuffer_Size = Framebuffer_Size<Getter>;

		template<bool Property_Access_Type>
		using Content_Scale = Window_Property<Property_Access_Type, enums::Window_Properties::Content_Scale, std::tuple<nullptr_t>, std::tuple<float&, float&>>;

		using Set_Content_Scale = Content_Scale<Setter>;
		using Get_Content_Scale = Content_Scale<Getter>;

		template<bool Property_Access_Type>
		using Size_Limits = Window_Property<Property_Access_Type, enums::Window_Properties::Size_Limits, std::tuple<const int32_t&, const int32_t&, const int32_t&, const int32_t&>, std::tuple<nullptr_t>>;

		using Set_Size_Limits = Size_Limits<Setter>;
		using Get_Size_Limits = Size_Limits<Getter>;

		template<bool Property_Access_Type>
		using Aspect_Ratio = Window_Property<Property_Access_Type, enums::Window_Properties::Aspect_Ratio, std::tuple<const int32_t&, const int32_t&>, std::tuple<nullptr_t>>;

		using Set_Aspect_Ratio = Aspect_Ratio<Setter>;
		using Get_Aspect_Ratio = Aspect_Ratio<Getter>;

		template<bool Property_Access_Type>
		using Position = Window_Property<Property_Access_Type, enums::Window_Properties::Position, std::tuple<const int32_t&, const int32_t&>, std::tuple<int32_t&, int32_t&>>;

		using Set_Position = Position<Setter>;
		using Get_Position = Position<Getter>;

		template<bool Property_Access_Type>
		using Title = Window_Property<Property_Access_Type, enums::Window_Properties::Title, std::tuple<const std::string&>, std::tuple<nullptr_t>>;

		using Set_Title = Title<Setter>;
		using Get_Title = Title<Getter>;

		template<bool Property_Access_Type>
		using Icon = Window_Property<Property_Access_Type, enums::Window_Properties::Icon, std::tuple<nullptr_t>, std::tuple<nullptr_t>>;

		using Set_Icon = Icon<Setter>;
		using Get_Icon = Icon<Getter>;

		template<bool Property_Access_Type>
		using Monitor = Window_Property<Property_Access_Type, enums::Window_Properties::Monitor, std::tuple<nullptr_t>, std::tuple<nullptr_t>>;

		using Set_Monitor = Monitor<Setter>;
		using Get_Monitor = Monitor<Getter>;

		template<bool Property_Access_Type>
		using Iconify = Window_Property<Property_Access_Type, enums::Window_Properties::Iconify, std::tuple<const bool&>, std::tuple<nullptr_t>>;

		using Set_Iconify = Iconify<Setter>;
		using Get_Iconify = Iconify<Getter>;

		template<bool Property_Access_Type>
		using Maximize = Window_Property<Property_Access_Type, enums::Window_Properties::Maximize, std::tuple<const bool&>, std::tuple<nullptr_t>>;

		using Set_Maximize = Maximize<Setter>;
		using Get_Maximize = Maximize<Getter>;

		template<bool Property_Access_Type>
		using Visible = Window_Property<Property_Access_Type, enums::Window_Properties::Visible, std::tuple<const bool&>, std::tuple<nullptr_t>>;

		using Set_Visible = Visible<Setter>;
		using Get_Visible = Visible<Getter>;

		template<bool Property_Access_Type>
		using Focus = Window_Property<Property_Access_Type, enums::Window_Properties::Focus, std::tuple<nullptr_t>, std::tuple<nullptr_t>>;

		using Set_Focus = Focus<Setter>;
		using Get_Focus = Focus<Getter>;

		template<bool Property_Access_Type>
		using Request_Attention = Window_Property<Property_Access_Type, enums::Window_Properties::Request_Attention, std::tuple<nullptr_t>, std::tuple<nullptr_t>>;

		using Set_Request_Attention = Request_Attention<Setter>;
		using Get_Request_Attention = Request_Attention<Getter>;

		template<bool Property_Access_Type>
		using Opacity = Window_Property<Property_Access_Type, enums::Window_Properties::Opacity, std::tuple<const float&>, std::tuple<nullptr_t>>;

		using Set_Opacity = Opacity<Setter>;
		using Get_Opacity = Opacity<Getter>;

		template<bool Property_Access_Type>
		using Attributes = Window_Property<Property_Access_Type, enums::Window_Properties::Attributes, std::tuple<const enums::Window_Attrubutes&, const std::int32_t&>, std::tuple<nullptr_t>>;

		using Set_Attributes = Attributes<Setter>;
		using Get_Attributes = Attributes<Getter>;

		template<bool Property_Access_Type>
		using Swap_Interval = Window_Property<Property_Access_Type, enums::Window_Properties::Swap_Interval, std::tuple<const std::int32_t&>, std::tuple<nullptr_t>>;

		using Set_Swap_Interval = Swap_Interval<Setter>;
		using Get_Swap_Interval = Swap_Interval<Getter>;

	} // properties

} // tilia

#endif // TILIA_WINDOW_PROPERTIES_HPP
