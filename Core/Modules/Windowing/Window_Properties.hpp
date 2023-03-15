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

	class Window;

	namespace utils
	{

		typedef struct GLFWwindow GLFWwindow; // GLFWwindow

	} // utils

	namespace windowing
	{

		namespace hints
		{



		} // hints

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

			template<enums::Window_Properties Property_Type, typename Set_Parameters, typename Get_Parameters>
			struct Window_Property {}; // Window_Property

			template<enums::Window_Properties Property_Type, typename... Set_Parameters, typename... Get_Parameters>
			struct Window_Property<Property_Type, std::tuple<Set_Parameters...>, std::tuple<Get_Parameters...>>
			{
			public:

				using Set_Parameters_Tuple = std::tuple<Set_Parameters...>;
				using Get_Parameters_Tuple = std::tuple<Get_Parameters...>;

			protected:
				std::tuple<Set_Parameters...> m_set_parameters;
				std::tuple<Get_Parameters...> m_get_parameters;
			public:
				static constexpr auto Type{ Property_Type };
				static constexpr auto Settable{ (sizeof...(Set_Parameters) > 0) };
				static constexpr auto Gettable{ (sizeof...(Get_Parameters) > 0) };

				virtual void Set_Property(Window& window) {};
				virtual void Get_Property(Window& window) {};
				auto Get_Properties()
				{
					return m_get_parameters;
				}
				auto Get_First_Property()
				{
					return std::get<0>(m_get_parameters);
				}
			}; // Window_Property

			struct Should_Close : public Window_Property<enums::Window_Properties::Should_Close, std::tuple<bool>, std::tuple<bool>>
			{
				Should_Close() = default;
				Should_Close(bool should_close) { m_set_parameters = should_close; }
				void Set_Property(Window& window) override;
				void Get_Property(Window& window) override;
			}; // Should_Close

			struct Size : public Window_Property<enums::Window_Properties::Size, std::tuple<std::int32_t, std::int32_t>, std::tuple<std::int32_t, std::int32_t>>
			{
				Size() = default;
				Size(std::int32_t width, std::int32_t height) { m_set_parameters = { width, height }; }
				void Set_Property(Window& window) override;
				void Get_Property(Window& window) override;
			}; // Size

			struct Frame_Size : public Window_Property<enums::Window_Properties::Frame_Size, std::tuple<>,
				std::tuple<std::int32_t, std::int32_t, std::int32_t, std::int32_t>>
			{
				Frame_Size() = default;
				void Get_Property(Window& window) override;
			}; // Frame_Size

			struct Framebuffer_Size : public Window_Property<
				enums::Window_Properties::Framebuffer_Size, std::tuple<>, std::tuple<std::int32_t, std::int32_t>>
			{
				Framebuffer_Size() = default;
				void Get_Property(Window& window) override;
			}; // Framebuffer_Size

			struct Content_Scale : public Window_Property<enums::Window_Properties::Content_Scale, std::tuple<>, std::tuple<float, float>>
			{
				Content_Scale() = default;
				void Get_Property(Window& window) override;
			}; // Content_Scale

			struct Size_Limits : public Window_Property<enums::Window_Properties::Size_Limits, 
				std::tuple<std::int32_t, std::int32_t, std::int32_t, std::int32_t>, std::tuple<>>
			{
				Size_Limits(std::int32_t min_width, std::int32_t min_height,
					std::int32_t max_width, std::int32_t max_height) 
				{ m_set_parameters = { min_width, min_height, max_width, max_height }; }
				void Set_Property(Window& window) override;
			}; // Size_Limits

			struct Aspect_Ratio : public Window_Property<enums::Window_Properties::Aspect_Ratio, 
				std::tuple<std::int32_t, std::int32_t>, std::tuple<>>
			{
				Aspect_Ratio(std::int32_t numer, std::int32_t denom) { m_set_parameters = { numer, denom }; }
				void Set_Property(Window& window) override;
			}; // Aspect_Ratio

			struct Position : public Window_Property<enums::Window_Properties::Position, 
				std::tuple<std::int32_t, std::int32_t>, std::tuple<std::int32_t, std::int32_t>>
			{
				Position() = default;
				Position(std::int32_t x_pos, std::int32_t y_pos) { m_set_parameters = { x_pos, y_pos }; }
				void Set_Property(Window& window) override;
				void Get_Property(Window& window) override;
			}; // Position

			struct Title : public Window_Property<enums::Window_Properties::Title, std::tuple<std::string>, std::tuple<>>
			{
				Title(std::string title) { m_set_parameters = title; }
				void Set_Property(Window& window) override;
			}; // Title

			struct Icon : public Window_Property<enums::Window_Properties::Icon, std::tuple<>, std::tuple<>>
			{
			}; // Icon

			struct Monitor : public Window_Property<enums::Window_Properties::Monitor, std::tuple<>, std::tuple<>>
			{
			}; // Monitor

			struct Iconify : public Window_Property<enums::Window_Properties::Iconify, std::tuple<bool>, std::tuple<bool>>
			{
				Iconify() = default;
				Iconify(bool iconify) { m_set_parameters = iconify; }
				void Set_Property(Window& window) override;
				void Get_Property(Window& window) override;
			}; // Iconify

			struct Maximize : public Window_Property<enums::Window_Properties::Maximize, std::tuple<bool>, std::tuple<bool>>
			{
				Maximize() = default;
				Maximize(bool maximize) { m_set_parameters = maximize; }
				void Set_Property(Window& window) override;
				void Get_Property(Window& window) override;
			}; // Maximize

			struct Visible : public Window_Property<enums::Window_Properties::Visible, std::tuple<bool>, std::tuple<bool>>
			{
				Visible() = default;
				Visible(bool visible) { m_set_parameters = visible; }
				void Set_Property(Window& window) override;
				void Get_Property(Window& window) override;
			}; // Visible

			struct Request_Attention : public Window_Property<enums::Window_Properties::Request_Attention, std::tuple<bool>, std::tuple<>>
			{
				Request_Attention(bool request_attention) { m_set_parameters = request_attention; }
				void Set_Property(Window& window) override;
			}; // Request_Attention

			struct Opacity : public Window_Property<enums::Window_Properties::Opacity, std::tuple<float>, std::tuple<float>>
			{
				Opacity() = default;
				Opacity(float opacity) { m_set_parameters = opacity; }
				void Set_Property(Window& window) override;
				void Get_Property(Window& window) override;
			}; // Opacity

			struct Swap_Interval : public Window_Property<enums::Window_Properties::Swap_Interval, std::tuple<std::int32_t>, std::tuple<std::int32_t>>
			{
				Swap_Interval() = default;
				Swap_Interval(std::int32_t swap_interval) { m_set_parameters = swap_interval; }
				void Set_Property(Window& window) override;
				void Get_Property(Window& window) override;
			}; // Swap_Interval

		} // properties

	} // windowing

} // tilia

#endif // TILIA_WINDOW_PROPERTIES_HPP
