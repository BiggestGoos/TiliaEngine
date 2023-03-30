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

			void Set_Hint(std::int32_t type, std::int32_t value);

			void Reset();

			inline void Resizable(bool resizable = true) { Set_Hint(*enums::Window_Hints::Resizable, resizable); }

			inline void Visible(bool visible = true) { Set_Hint(*enums::Window_Hints::Visible, visible); }

			inline void Decorated(bool decorated = true) { Set_Hint(*enums::Window_Hints::Decorated, decorated); }

			inline void Focused(bool focused = true) { Set_Hint(*enums::Window_Hints::Focused, focused); }

			inline void Auto_Iconify(bool auto_iconify = true) { Set_Hint(*enums::Window_Hints::Auto_Iconify, auto_iconify); }

			inline void Floating(bool floating = false) { Set_Hint(*enums::Window_Hints::Floating, floating); }

			inline void Maximized(bool maximized = false) { Set_Hint(*enums::Window_Hints::Maximized, maximized); }

			inline void Center_Cursor(bool center_cursor = true) { Set_Hint(*enums::Window_Hints::Center_Cursor, center_cursor); }

			inline void Transparent_Framebuffer(bool transparent_framebuffer = false) { Set_Hint(*enums::Window_Hints::Transparent_Framebuffer, transparent_framebuffer); }

			inline void Focus_On_Show(bool focus_on_show = true) { Set_Hint(*enums::Window_Hints::Focus_On_Show, focus_on_show); }

			inline void Scale_To_Monitor(bool scale_to_monitor = false) { Set_Hint(*enums::Window_Hints::Scale_To_Monitor, scale_to_monitor); }

			inline void Refresh_Rate(std::int32_t refresh_rate = DO_NOT_CARE) { Set_Hint(*enums::Window_Hints::Refresh_Rate, refresh_rate); }

			namespace framebuffer
			{

				inline void Red_Bits(std::int32_t red_bits = 8) { Set_Hint(*enums::Framebuffer_Hints::Red_Bits, red_bits); }

				inline void Green_Bits(std::int32_t green_bits = 8) { Set_Hint(*enums::Framebuffer_Hints::Green_Bits, green_bits); }

				inline void Blue_Bits(std::int32_t blue_bits = 8) { Set_Hint(*enums::Framebuffer_Hints::Blue_Bits, blue_bits); }

				inline void Alpha_Bits(std::int32_t alpha_bits = 8) { Set_Hint(*enums::Framebuffer_Hints::Alpha_Bits, alpha_bits); }

				inline void Depth_Bits(std::int32_t depth_bits = 24) { Set_Hint(*enums::Framebuffer_Hints::Depth_Bits, depth_bits); }

				inline void Stencil_Bits(std::int32_t stencil_bits = 8) { Set_Hint(*enums::Framebuffer_Hints::Stencil_Bits, stencil_bits); }

				inline void Accum_Red_Bits(std::int32_t accum_red_bits = 0) { Set_Hint(*enums::Framebuffer_Hints::Accum_Red_Bits, accum_red_bits); }

				inline void Accum_Green_Bits(std::int32_t accum_green_bits = 0) { Set_Hint(*enums::Framebuffer_Hints::Accum_Green_Bits, accum_green_bits); }

				inline void Accum_Blue_Bits(std::int32_t accum_blue_bits = 0) { Set_Hint(*enums::Framebuffer_Hints::Accum_Blue_Bits, accum_blue_bits); }

				inline void Accum_Alpha_Bits(std::int32_t accum_alpha_bits = 0) { Set_Hint(*enums::Framebuffer_Hints::Accum_Alpha_Bits, accum_alpha_bits); }

				inline void AUX_Buffers(std::int32_t AUX_buffers = 0) { Set_Hint(*enums::Framebuffer_Hints::AUX_Buffers, AUX_buffers); }

				inline void Stereo(bool stereo = false) { Set_Hint(*enums::Framebuffer_Hints::Stereo, stereo); }

				inline void Samples(std::int32_t samples = 0) { Set_Hint(*enums::Framebuffer_Hints::Samples, samples); }

				inline void SRGB_Capable(bool SRGB_capable = false) { Set_Hint(*enums::Framebuffer_Hints::SRGB_Capable, SRGB_capable); }

				inline void Doublebuffer(bool doublebuffer = true) { Set_Hint(*enums::Framebuffer_Hints::Doublebuffer, doublebuffer); }

			} // framebuffer

			namespace context
			{

				inline void Client_API(enums::Client_API client_API = enums::Client_API::OpenGL) { Set_Hint(*enums::Context_Hints::Client_API, *client_API); }

				inline void Creation_API(enums::Context_Creation_API context_creation_API = enums::Context_Creation_API::Native) { Set_Hint(*enums::Context_Hints::Creation_API, *context_creation_API); }

				inline void Verion_Major(std::int32_t context_verion_major = 1) { Set_Hint(*enums::Context_Hints::Verion_Major, context_verion_major); }

				inline void Verion_Minor(std::int32_t context_verion_minor = 0) { Set_Hint(*enums::Context_Hints::Verion_Minor, context_verion_minor); }

				inline void OpenGL_Forward_Compat(bool openGL_forward_compat = false) { Set_Hint(*enums::Context_Hints::OpenGL_Forward_Compat, openGL_forward_compat); }

				inline void OpenGL_Debug_Context(bool openGL_debug_context = false) { Set_Hint(*enums::Context_Hints::OpenGL_Debug_Context, openGL_debug_context); }

				inline void OpenGL_Profile(enums::OpenGL_Profile openGL_profile = enums::OpenGL_Profile::Any) { Set_Hint(*enums::Context_Hints::OpenGL_Profile, *openGL_profile); }

				inline void Robustness(enums::Context_Robustness context_robustness = enums::Context_Robustness::No_Robustness) { Set_Hint(*enums::Context_Hints::Robustness, *context_robustness); }

				inline void Release_Behavior(enums::Context_Release_Behavior context_release_behavior = enums::Context_Release_Behavior::Any) { Set_Hint(*enums::Context_Hints::Release_Behavior, *context_release_behavior); }

			} // context

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
			struct Window_Property_ {};

			template<enums::Window_Properties Property_Type, typename... Set_Parameters, typename... Get_Parameters>
			struct Window_Property_<Property_Type, std::tuple<Set_Parameters...>, std::tuple<Get_Parameters...>>
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

				template<bool multiple_values>
				auto Get_Value() { }

				template<>
				auto Get_Value<true>()
				{
					return m_get_parameters;
				}
				template<>
				auto Get_Value<false>()
				{
					return std::get<0>(m_get_parameters);
				}
			}; // Window_Property_

			template<enums::Window_Properties Type>
			struct Window_Property {};

			template<>
			struct Window_Property<enums::Window_Properties::Should_Close>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Size>
			{
				using Set_Parameters = std::tuple<std::int32_t, std::int32_t>;
				using Get_Parameters = std::tuple<std::int32_t, std::int32_t>;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Frame_Size>
			{
				using Get_Parameters = std::tuple<std::int32_t, std::int32_t, std::int32_t, std::int32_t>;
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Framebuffer_Size>
			{
				using Get_Parameters = std::tuple<std::int32_t, std::int32_t>;
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Content_Scale>
			{
				using Get_Parameters = std::tuple<float, float>;
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Size_Limits>
			{
				using Set_Parameters = std::tuple<std::int32_t, std::int32_t, std::int32_t, std::int32_t>;
				static void Set(Window& window, Set_Parameters&& parameters);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Aspect_Ratio>
			{
				using Set_Parameters = std::tuple<std::int32_t, std::int32_t>;
				static void Set(Window& window, Set_Parameters&& parameters);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Position>
			{
				using Set_Parameters = std::tuple<std::int32_t, std::int32_t>;
				using Get_Parameters = std::tuple<std::int32_t, std::int32_t>;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Title>
			{
				using Set_Parameters = std::string;
				using Get_Parameters = std::string;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			// Todo: When image loading is done, implement this
			template<>
			struct Window_Property<enums::Window_Properties::Icon>
			{
			};

			// Todo: When monitor abstraction is done, implement this
			template<>
			struct Window_Property<enums::Window_Properties::Monitor>
			{
			};

			/*
			struct Underlying_Window : public Window_Property_<enums::Window_Properties::Underlying_Window, std::tuple<>, std::tuple<GLFWwindow*>>
			{
				Underlying_Window() = default;
				void Get(Window& window);
			};

			struct Should_Close : public Window_Property_<enums::Window_Properties::Should_Close, std::tuple<bool>, std::tuple<bool>>
			{
				Should_Close() = default;
				Should_Close(bool should_close) { m_set_parameters = should_close; }
				void Set(Window& window);
				void Get(Window& window);
			};

			struct Size : public Window_Property_<enums::Window_Properties::Size, std::tuple<std::int32_t, std::int32_t>, std::tuple<std::int32_t, std::int32_t>>
			{
				Size() = default;
				Size(std::int32_t width, std::int32_t height) { m_set_parameters = { width, height }; }
				void Set(Window& window);
				void Get(Window& window);
			};

			struct Frame_Size : public Window_Property_<enums::Window_Properties::Frame_Size, std::tuple<>,
				std::tuple<std::int32_t, std::int32_t, std::int32_t, std::int32_t>>
			{
				Frame_Size() = default;
				void Get(Window& window);
			};

			struct Framebuffer_Size : public Window_Property_<
				enums::Window_Properties::Framebuffer_Size, std::tuple<>, std::tuple<std::int32_t, std::int32_t>>
			{
				Framebuffer_Size() = default;
				void Get(Window& window);
			};
			*/

			/*

			struct Content_Scale : public Window_Property_<enums::Window_Properties::Content_Scale, std::tuple<>, std::tuple<float, float>>
			{
				Content_Scale() = default;
				void Get(Window& window);
			};

			struct Size_Limits : public Window_Property_<enums::Window_Properties::Size_Limits, 
				std::tuple<std::int32_t, std::int32_t, std::int32_t, std::int32_t>, std::tuple<>>
			{
				Size_Limits(std::int32_t min_width, std::int32_t min_height,
					std::int32_t max_width, std::int32_t max_height) 
				{ m_set_parameters = { min_width, min_height, max_width, max_height }; }
				void Set(Window& window);
			};

			struct Aspect_Ratio : public Window_Property_<enums::Window_Properties::Aspect_Ratio, 
				std::tuple<std::int32_t, std::int32_t>, std::tuple<>>
			{
				Aspect_Ratio(std::int32_t numer, std::int32_t denom) { m_set_parameters = { numer, denom }; }
				void Set(Window& window);
			};

			struct Position : public Window_Property_<enums::Window_Properties::Position, 
				std::tuple<std::int32_t, std::int32_t>, std::tuple<std::int32_t, std::int32_t>>
			{
				Position() = default;
				Position(std::int32_t x_pos, std::int32_t y_pos) { m_set_parameters = { x_pos, y_pos }; }
				void Set(Window& window);
				void Get(Window& window);
			};

			struct Title : public Window_Property_<enums::Window_Properties::Title, std::tuple<std::string>, std::tuple<std::string>>
			{
				Title() = default;
				Title(std::string title) { m_set_parameters = title; }
				void Set(Window& window);
				void Get(Window& window);
			};

			// Todo: When image loading is done, implement this
			struct Icon : public Window_Property_<enums::Window_Properties::Icon, std::tuple<>, std::tuple<>>
			{
			};

			struct Monitor : public Window_Property_<enums::Window_Properties::Monitor, std::tuple<>, std::tuple<>>
			{
			};

			struct Iconify : public Window_Property_<enums::Window_Properties::Iconify, std::tuple<bool>, std::tuple<bool>>
			{
				Iconify() = default;
				Iconify(bool iconify) { m_set_parameters = iconify; }
				void Set(Window& window);
				void Get(Window& window);
			};

			struct Maximize : public Window_Property_<enums::Window_Properties::Maximize, std::tuple<bool>, std::tuple<bool>>
			{
				Maximize() = default;
				Maximize(bool maximize) { m_set_parameters = maximize; }
				void Set(Window& window);
				void Get(Window& window);
			};

			struct Visible : public Window_Property_<enums::Window_Properties::Visible, std::tuple<bool>, std::tuple<bool>>
			{
				Visible() = default;
				Visible(bool visible) { m_set_parameters = visible; }
				void Set(Window& window);
				void Get(Window& window);
			};

			struct Focus : public Window_Property_<enums::Window_Properties::Focus, std::tuple<bool>, std::tuple<bool>>
			{
				Focus() { m_set_parameters = true; }
				void Set(Window& window);
				void Get(Window& window);
			};

			struct Request_Attention : public Window_Property_<enums::Window_Properties::Request_Attention, std::tuple<bool>, std::tuple<>>
			{
				Request_Attention() { m_set_parameters = true; }
				void Set(Window& window);
			};

			struct Opacity : public Window_Property_<enums::Window_Properties::Opacity, std::tuple<float>, std::tuple<float>>
			{
				Opacity() = default;
				Opacity(float opacity) { m_set_parameters = opacity; }
				void Set(Window& window);
				void Get(Window& window);
			};

			struct Swap_Interval : public Window_Property_<enums::Window_Properties::Swap_Interval, std::tuple<std::int32_t>, std::tuple<std::int32_t>>
			{
				Swap_Interval() = default;
				Swap_Interval(std::int32_t swap_interval) { m_set_parameters = swap_interval; }
				void Set(Window& window);
				void Get(Window& window);
			};

			struct Resizeable : public Window_Property_<enums::Window_Properties::Resizable, std::tuple<bool>, std::tuple<bool>>
			{
				Resizeable() = default;
				Resizeable(bool resizeable) { m_set_parameters = resizeable; }
				void Set(Window& window);
				void Get(Window& window);
			};

			struct Decorated : public Window_Property_<enums::Window_Properties::Decorated, std::tuple<bool>, std::tuple<bool>>
			{
				Decorated() = default;
				Decorated(bool decorated) { m_set_parameters = decorated; }
				void Set(Window& window);
				void Get(Window& window);
			};

			struct Auto_Iconify : public Window_Property_<enums::Window_Properties::Auto_Iconify, std::tuple<bool>, std::tuple<bool>>
			{
				Auto_Iconify() = default;
				Auto_Iconify(bool auto_iconify) { m_set_parameters = auto_iconify; }
				void Set(Window& window);
				void Get(Window& window);
			};

			struct Floating : public Window_Property_<enums::Window_Properties::Floating, std::tuple<bool>, std::tuple<bool>>
			{
				Floating() = default;
				Floating(bool floating) { m_set_parameters = floating; }
				void Set(Window& window);
				void Get(Window& window);
			};

			struct Focus_On_Show : public Window_Property_<enums::Window_Properties::Focus_On_Show, std::tuple<bool>, std::tuple<bool>>
			{
				Focus_On_Show() = default;
				Focus_On_Show(bool focus_on_show) { m_set_parameters = focus_on_show; }
				void Set(Window& window);
				void Get(Window& window);
			};

			struct Transparent_Framebuffer : public Window_Property_<enums::Window_Properties::Transparent_Framebuffer, std::tuple<>, std::tuple<bool>>
			{
				Transparent_Framebuffer() = default;
				void Get(Window& window);
			};

			// Todo: When framebuffer is done, implement this
			struct Framebuffer : public Window_Property_<enums::Window_Properties::Framebuffer, std::tuple<>, std::tuple<>>
			{
			};

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
					void Get(Window& window);
				};

				struct Creation_API : public Context_Property<enums::Context_Properties::Creation_API, enums::Context_Creation_API>
				{
					Creation_API() = default;
					void Get(Window& window);
				};

				struct Version : public Context_Property<enums::Context_Properties::Version, std::uint32_t, std::uint32_t, std::uint32_t>
				{
					Version() = default;
					void Get(Window& window);
				};

				struct OpenGL_Forward_Compatible : public Context_Property<enums::Context_Properties::OpenGL_Forward_Compatible, bool>
				{
					OpenGL_Forward_Compatible() = default;
					void Get(Window& window);
				};

				struct OpenGL_Debug_Context : public Context_Property<enums::Context_Properties::OpenGL_Debug_Context, bool>
				{
					OpenGL_Debug_Context() = default;
					void Get(Window& window);
				};

				struct OpenGL_Profile : public Context_Property<enums::Context_Properties::OpenGL_Profile, enums::OpenGL_Profile>
				{
					OpenGL_Profile() = default;
					void Get(Window& window);
				};

				struct Release_Behavior : public Context_Property<enums::Context_Properties::Release_Behavior, enums::Context_Release_Behavior>
				{
					Release_Behavior() = default;
					void Get(Window& window);
				};

				struct No_Error : public Context_Property<enums::Context_Properties::No_Error, bool>
				{
					No_Error() = default;
					void Get(Window& window);
				};

				struct Robustness : public Context_Property<enums::Context_Properties::Robustness, enums::Context_Robustness>
				{
					Robustness() = default;
					void Get(Window& window);
				};

			} // context

			*/

		} // properties

	} // windowing

} // tilia

#endif // TILIA_WINDOW_PROPERTIES_HPP
