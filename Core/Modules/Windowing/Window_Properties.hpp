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

	namespace windowing
	{

		typedef struct GLFWwindow GLFWwindow; // GLFWwindow

		class Window; // Window

		namespace hints
		{

			//template<enums::Window_Hints Hint_Type, typename... Set_Parameters>
			//struct Hint
			//{
			//public:
			//	using Set_Parameters_Tuple = std::tuple<Set_Parameters...>;
			//protected:
			//	Set_Parameters_Tuple m_set_parameters;
			//public:
			//	static constexpr auto Type{ Hint_Type };
			//}; // Hint

			//template<enums::Window_Hints Hint_Type>
			//struct Bool_Hint : public Hint<Hint_Type, bool>
			//{
			//	Bool_Hint(bool value) : m_set_parameters{ value } {}
			//}; // Bool_Hint

			//struct Focused : public Bool_Hint<enums::Window_Hints::Focused>
			//{
			//	using Base_Type = Bool_Hint<enums::Window_Hints::Focused>
			//	Focused(bool focused);
			//}; // Foucesed

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
				void(GLFWwindow*, std::int32_t, std::int32_t)>;

			using Size = Window_Func<enums::Window_Callbacks::Size,
				void(GLFWwindow*, std::int32_t, std::int32_t)>;

			using Close = Window_Func<enums::Window_Callbacks::Close,
				void(GLFWwindow*)>;

			using Refresh = Window_Func<enums::Window_Callbacks::Refresh,
				void(GLFWwindow*)>;

			using Focus = Window_Func<enums::Window_Callbacks::Focus,
				void(GLFWwindow*, std::int32_t)>;

			using Inconify = Window_Func<enums::Window_Callbacks::Iconify,
				void(GLFWwindow*, std::int32_t)>;

			using Maximize = Window_Func<enums::Window_Callbacks::Maximize,
				void(GLFWwindow*, std::int32_t)>;

			using Framebuffer_Size = Window_Func<enums::Window_Callbacks::Framebuffer_Size,
				void(GLFWwindow*, std::int32_t, std::int32_t)>;

			using Content_Scale = Window_Func<enums::Window_Callbacks::Content_Scale,
				void(GLFWwindow*, float, float)>;

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
				Set_Parameters_Tuple m_set_parameters;
				Get_Parameters_Tuple m_get_parameters;
			public:
				static constexpr auto Type{ Property_Type };
				static constexpr auto Settable{ (sizeof...(Set_Parameters) > 0) };
				static constexpr auto Gettable{ (sizeof...(Get_Parameters) > 0) };

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
				void Set_Property(Window& window);
				void Get_Property(Window& window);
			}; // Should_Close

			struct Size : public Window_Property<enums::Window_Properties::Size, std::tuple<std::int32_t, std::int32_t>, std::tuple<std::int32_t, std::int32_t>>
			{
				Size() = default;
				Size(std::int32_t width, std::int32_t height) { m_set_parameters = { width, height }; }
				void Set_Property(Window& window);
				void Get_Property(Window& window);
			}; // Size

			struct Frame_Size : public Window_Property<enums::Window_Properties::Frame_Size, std::tuple<>,
				std::tuple<std::int32_t, std::int32_t, std::int32_t, std::int32_t>>
			{
				Frame_Size() = default;
				void Get_Property(Window& window);
			}; // Frame_Size

			struct Framebuffer_Size : public Window_Property<
				enums::Window_Properties::Framebuffer_Size, std::tuple<>, std::tuple<std::int32_t, std::int32_t>>
			{
				Framebuffer_Size() = default;
				void Get_Property(Window& window);
			}; // Framebuffer_Size

			struct Content_Scale : public Window_Property<enums::Window_Properties::Content_Scale, std::tuple<>, std::tuple<float, float>>
			{
				Content_Scale() = default;
				void Get_Property(Window& window);
			}; // Content_Scale

			struct Size_Limits : public Window_Property<enums::Window_Properties::Size_Limits, 
				std::tuple<std::int32_t, std::int32_t, std::int32_t, std::int32_t>, std::tuple<>>
			{
				Size_Limits(std::int32_t min_width, std::int32_t min_height,
					std::int32_t max_width, std::int32_t max_height) 
				{ m_set_parameters = { min_width, min_height, max_width, max_height }; }
				void Set_Property(Window& window);
			}; // Size_Limits

			struct Aspect_Ratio : public Window_Property<enums::Window_Properties::Aspect_Ratio, 
				std::tuple<std::int32_t, std::int32_t>, std::tuple<>>
			{
				Aspect_Ratio(std::int32_t numer, std::int32_t denom) { m_set_parameters = { numer, denom }; }
				void Set_Property(Window& window);
			}; // Aspect_Ratio

			struct Position : public Window_Property<enums::Window_Properties::Position, 
				std::tuple<std::int32_t, std::int32_t>, std::tuple<std::int32_t, std::int32_t>>
			{
				Position() = default;
				Position(std::int32_t x_pos, std::int32_t y_pos) { m_set_parameters = { x_pos, y_pos }; }
				void Set_Property(Window& window);
				void Get_Property(Window& window);
			}; // Position

			struct Title : public Window_Property<enums::Window_Properties::Title, std::tuple<std::string>, std::tuple<std::string>>
			{
				Title() = default;
				Title(std::string title) { m_set_parameters = title; }
				void Set_Property(Window& window);
				void Get_Property(Window& window);
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
				void Set_Property(Window& window);
				void Get_Property(Window& window);
			}; // Iconify

			struct Maximize : public Window_Property<enums::Window_Properties::Maximize, std::tuple<bool>, std::tuple<bool>>
			{
				Maximize() = default;
				Maximize(bool maximize) { m_set_parameters = maximize; }
				void Set_Property(Window& window);
				void Get_Property(Window& window);
			}; // Maximize

			struct Visible : public Window_Property<enums::Window_Properties::Visible, std::tuple<bool>, std::tuple<bool>>
			{
				Visible() = default;
				Visible(bool visible) { m_set_parameters = visible; }
				void Set_Property(Window& window);
				void Get_Property(Window& window);
			}; // Visible

			struct Focus : public Window_Property<enums::Window_Properties::Focus, std::tuple<bool>, std::tuple<bool>>
			{
				Focus() { m_set_parameters = true; }
				void Set_Property(Window& window);
				void Get_Property(Window& window);
			}; // Focus

			struct Request_Attention : public Window_Property<enums::Window_Properties::Request_Attention, std::tuple<bool>, std::tuple<>>
			{
				Request_Attention() { m_set_parameters = true; }
				void Set_Property(Window& window);
			}; // Attention_Request

			struct Opacity : public Window_Property<enums::Window_Properties::Opacity, std::tuple<float>, std::tuple<float>>
			{
				Opacity() = default;
				Opacity(float opacity) { m_set_parameters = opacity; }
				void Set_Property(Window& window);
				void Get_Property(Window& window);
			}; // Opacity

			struct Swap_Interval : public Window_Property<enums::Window_Properties::Swap_Interval, std::tuple<std::int32_t>, std::tuple<std::int32_t>>
			{
				Swap_Interval() = default;
				Swap_Interval(std::int32_t swap_interval) { m_set_parameters = swap_interval; }
				void Set_Property(Window& window);
				void Get_Property(Window& window);
			}; // Swap_Interval

			struct Resizeable : public Window_Property<enums::Window_Properties::Resizable, std::tuple<bool>, std::tuple<bool>>
			{
				Resizeable() = default;
				Resizeable(bool resizeable) { m_set_parameters = resizeable; }
				void Set_Property(Window& window);
				void Get_Property(Window& window);
			}; // Resizeable

			struct Decorated : public Window_Property<enums::Window_Properties::Decorated, std::tuple<bool>, std::tuple<bool>>
			{
				Decorated() = default;
				Decorated(bool decorated) { m_set_parameters = decorated; }
				void Set_Property(Window& window);
				void Get_Property(Window& window);
			}; // Decorated

			struct Auto_Iconify : public Window_Property<enums::Window_Properties::Auto_Iconify, std::tuple<bool>, std::tuple<bool>>
			{
				Auto_Iconify() = default;
				Auto_Iconify(bool auto_iconify) { m_set_parameters = auto_iconify; }
				void Set_Property(Window& window);
				void Get_Property(Window& window);
			}; // Auto_Iconify

			struct Floating : public Window_Property<enums::Window_Properties::Floating, std::tuple<bool>, std::tuple<bool>>
			{
				Floating() = default;
				Floating(bool floating) { m_set_parameters = floating; }
				void Set_Property(Window& window);
				void Get_Property(Window& window);
			}; // Floating

			struct Focus_On_Show : public Window_Property<enums::Window_Properties::Focus_On_Show, std::tuple<bool>, std::tuple<bool>>
			{
				Focus_On_Show() = default;
				Focus_On_Show(bool focus_on_show) { m_set_parameters = focus_on_show; }
				void Set_Property(Window& window);
				void Get_Property(Window& window);
			}; // Focus_On_Show

			struct Transparent_Framebuffer : public Window_Property<enums::Window_Properties::Transparent_Framebuffer, std::tuple<>, std::tuple<bool>>
			{
				Transparent_Framebuffer() = default;
				void Get_Property(Window& window);
			}; // Transparent_Framebuffer

			struct Framebuffer : public Window_Property<enums::Window_Properties::Framebuffer, std::tuple<>, std::tuple<>>
			{
			}; // Framebuffer

			namespace context
			{

				template<enums::Context_Properties Property_Type, typename... Get_Parameters>
				struct Context_Property
				{
				public:

					using Get_Parameters_Tuple = std::tuple<Get_Parameters...>;

				protected:
					std::tuple<Get_Parameters...> m_get_parameters;
				public:
					static constexpr auto Type{ Property_Type };
					static constexpr auto Gettable{ true };

					auto Get_Properties()
					{
						return m_get_parameters;
					}
					auto Get_First_Property()
					{
						return std::get<0>(m_get_parameters);
					}
				}; // Context_Property

				struct Client_API : public Context_Property<enums::Context_Properties::Client_API, enums::Client_API>
				{
					Client_API() = default;
					void Get_Property(Window& window);
				}; // Client_API

				struct Creation_API : public Context_Property<enums::Context_Properties::Creation_API, enums::Context_Creation_API>
				{
					Creation_API() = default;
					void Get_Property(Window& window);
				}; // Creation_API

				struct Version : public Context_Property<enums::Context_Properties::Version, std::uint32_t, std::uint32_t, std::uint32_t>
				{
					Version() = default;
					void Get_Property(Window& window);
				}; // Version

				struct OpenGL_Forward_Compatible : public Context_Property<enums::Context_Properties::OpenGL_Forward_Compatible, bool>
				{
					OpenGL_Forward_Compatible() = default;
					void Get_Property(Window& window);
				}; // OpenGL_Forward_Compatible

				struct OpenGL_Debug_Context : public Context_Property<enums::Context_Properties::OpenGL_Debug_Context, bool>
				{
					OpenGL_Debug_Context() = default;
					void Get_Property(Window& window);
				}; // OpenGL_Debug_Context

				struct OpenGL_Profile : public Context_Property<enums::Context_Properties::OpenGL_Profile, enums::OpenGL_Profile>
				{
					OpenGL_Profile() = default;
					void Get_Property(Window& window);
				}; // OpenGL_Profile

				struct Release_Behavior : public Context_Property<enums::Context_Properties::Release_Behavior, enums::Context_Release_Behavior>
				{
					Release_Behavior() = default;
					void Get_Property(Window& window);
				}; // Release_Behavior

				struct No_Error : public Context_Property<enums::Context_Properties::No_Error, bool>
				{
					No_Error() = default;
					void Get_Property(Window& window);
				}; // No_Error

				struct Robustness : public Context_Property<enums::Context_Properties::Robustness, enums::Context_Robustness>
				{
					Robustness() = default;
					void Get_Property(Window& window);
				}; // Robustness

			} // context

		} // properties

	} // windowing

} // tilia

#endif // TILIA_WINDOW_PROPERTIES_HPP
