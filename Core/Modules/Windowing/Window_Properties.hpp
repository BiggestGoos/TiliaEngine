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
#include TILIA_MONITOR_INCLUDE

namespace tilia
{

	namespace windowing
	{

		typedef struct GLFWwindow GLFWwindow; // GLFWwindow

		class Window; // Window

		namespace enums
		{
			// Window hints which can be used to set different values used in creating windows.
			// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
			enum class Hints {
				Resizable = 0x00020003,
				Visible = 0x00020004,
				Decorated = 0x00020005,
				Focused = 0x00020001,
				Auto_Iconify = 0x00020006,
				Floating = 0x00020007,
				Maximized = 0x00020008,
				Center_Cursor = 0x00020009,
				Transparent_Framebuffer = 0x0002000A,
				Focus_On_Show = 0x0002000C,
				Scale_To_Monitor = 0x0002200C,
				Refresh_Rate = 0x0002100F
			}; // Hints
			// Windowing callbacks.
			// Check this out for info: https://www.glfw.org/docs/3.3/group__window.html
			enum class Callbacks
			{
				Position = 0x0000,
				Size = 0x0001,
				Close = 0x0002,
				Refresh = 0x0003,
				Focus = 0x0004,
				Iconify = 0x0005,
				Maximize = 0x0006,
				Framebuffer_Size = 0x0007,
				Content_Scale = 0x0008
			}; // Callbacks
			// Window properties.
			// Check this out for info: https://www.glfw.org/docs/3.3/window_guide.html
			enum class Properties
			{
				Underlying_Window = 0x0000,
				Should_Close = 0x0001,
				Size = 0x0002,
				Frame_Size = 0x0003,
				Framebuffer_Size = 0x0004,
				Content_Scale = 0x0005,
				Size_Limits = 0x0006,
				Aspect_Ratio = 0x0007,
				Position = 0x0008,
				Title = 0x0009,
				Icon = 0x000A,
				Monitor = 0x000B,
				Fullscreen = 0x000C,
				Iconify = 0x000D,
				Maximize = 0x000E,
				Visible = 0x000F,
				Focus = 0x0010,
				Request_Attention = 0x0011,
				Opacity = 0x0012,
				Swap_Interval = 0x0013,
				Resizable = 0x0014,
				Decorated = 0x0015,
				Auto_Iconify = 0x0016,
				Floating = 0x0017,
				Focus_On_Show = 0x0018,
				Transparent_Framebuffer = 0x0019,
				Framebuffer = 0x001A
			}; // Properties
			namespace framebuffer
			{
				// Framebuffer hints which can be used to set different values used in creating windows.
				// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
				enum class Hints
				{
					Red_Bits = 0x00021001,
					Green_Bits = 0x00021002,
					Blue_Bits = 0x00021003,
					Alpha_Bits = 0x00021004,
					Depth_Bits = 0x00021005,
					Stencil_Bits = 0x00021006,
					Accum_Red_Bits = 0x00021007,
					Accum_Green_Bits = 0x00021008,
					Accum_Blue_Bits = 0x00021009,
					Accum_Alpha_Bits = 0x0002100A,
					AUX_Buffers = 0x0002100B,
					Stereo = 0x0002100C,
					Samples = 0x0002100D,
					SRGB_Capable = 0x0002100E,
					Doublebuffer = 0x00021010,
				}; // Hints
			} // framebuffer
			namespace context
			{
				// Contexts hints which can be used to set different values used in creating windows.
				// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
				enum class Hints
				{
					Client_API = 0x00022001,
					Creation_API = 0x0002200B,
					Verion_Major = 0x00022002,
					Verion_Minor = 0x00022003,
					OpenGL_Forward_Compat = 0x00022006,
					OpenGL_Debug_Context = 0x00022007,
					OpenGL_Profile = 0x00022008,
					Robustness = 0x00022005,
					Release_Behavior = 0x00022009,
				}; // Hints
				// Context properties.
				// Check this out for info: https://www.glfw.org/docs/3.3/window_guide.html
				enum class Properties
				{
					Client_API = 0x0000,
					Creation_API = 0x0001,
					Version = 0x0002,
					OpenGL_Forward_Compatible = 0x0003,
					OpenGL_Debug_Context = 0x0004,
					OpenGL_Profile = 0x0005,
					Release_Behavior = 0x0006,
					No_Error = 0x0007,
					Robustness = 0x0008
				}; // Properties
				// What graphics API to use.
				// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
				enum class Client_API
				{
					No_API = 0x00000000,
					OpenGL = 0x00030001,
					OpenGL_ES = 0x00030002
				}; // Client_API
				// What API to create contexts for openGL.
				// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
				enum class Creation_API
				{
					Native = 0x00036001,
					EGL = 0x00036002,
					OSMesa = 0x00036003
				}; // Creation_API
				// The robustness of the created context when creating a window.
				// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
				enum class Robustness
				{
					No_Robustness = 0x00000000,
					No_Reset_Notification = 0x00031001,
					Lose_Context_On_Reset = 0x00031002
				}; // Robustness
				// What happens when the context is released.
				// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
				enum class Release_Behavior
				{
					Any = 0x00000000,
					Flush = 0x00035001,
					None = 0x00035002
				}; // Release_Behavior
				// The profile of the openGL context.
				// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
				enum class OpenGL_Profile
				{
					Any = 0x00000000,
					Core = 0x00032001,
					Compat = 0x00032002
				}; // OpenGL_Profile
			} // context
		} // enums

		namespace hints
		{

			void Set_Hint(std::int32_t type, std::int32_t value);

			void Reset();

			inline void Resizable(bool resizable = true) { Set_Hint(*enums::Hints::Resizable, resizable); }

			inline void Visible(bool visible = true) { Set_Hint(*enums::Hints::Visible, visible); }

			inline void Decorated(bool decorated = true) { Set_Hint(*enums::Hints::Decorated, decorated); }

			inline void Focused(bool focused = true) { Set_Hint(*enums::Hints::Focused, focused); }

			inline void Auto_Iconify(bool auto_iconify = true) { Set_Hint(*enums::Hints::Auto_Iconify, auto_iconify); }

			inline void Floating(bool floating = false) { Set_Hint(*enums::Hints::Floating, floating); }

			inline void Maximized(bool maximized = false) { Set_Hint(*enums::Hints::Maximized, maximized); }

			inline void Center_Cursor(bool center_cursor = true) { Set_Hint(*enums::Hints::Center_Cursor, center_cursor); }

			inline void Transparent_Framebuffer(bool transparent_framebuffer = false) { Set_Hint(*enums::Hints::Transparent_Framebuffer, transparent_framebuffer); }

			inline void Focus_On_Show(bool focus_on_show = true) { Set_Hint(*enums::Hints::Focus_On_Show, focus_on_show); }

			inline void Scale_To_Monitor(bool scale_to_monitor = false) { Set_Hint(*enums::Hints::Scale_To_Monitor, scale_to_monitor); }

			inline void Refresh_Rate(std::int32_t refresh_rate = DO_NOT_CARE) { Set_Hint(*enums::Hints::Refresh_Rate, refresh_rate); }

			namespace framebuffer
			{

				inline void Red_Bits(std::int32_t red_bits = 8) { Set_Hint(*enums::framebuffer::Hints::Red_Bits, red_bits); }

				inline void Green_Bits(std::int32_t green_bits = 8) { Set_Hint(*enums::framebuffer::Hints::Green_Bits, green_bits); }

				inline void Blue_Bits(std::int32_t blue_bits = 8) { Set_Hint(*enums::framebuffer::Hints::Blue_Bits, blue_bits); }

				inline void Alpha_Bits(std::int32_t alpha_bits = 8) { Set_Hint(*enums::framebuffer::Hints::Alpha_Bits, alpha_bits); }

				inline void Depth_Bits(std::int32_t depth_bits = 24) { Set_Hint(*enums::framebuffer::Hints::Depth_Bits, depth_bits); }

				inline void Stencil_Bits(std::int32_t stencil_bits = 8) { Set_Hint(*enums::framebuffer::Hints::Stencil_Bits, stencil_bits); }

				inline void Accum_Red_Bits(std::int32_t accum_red_bits = 0) { Set_Hint(*enums::framebuffer::Hints::Accum_Red_Bits, accum_red_bits); }

				inline void Accum_Green_Bits(std::int32_t accum_green_bits = 0) { Set_Hint(*enums::framebuffer::Hints::Accum_Green_Bits, accum_green_bits); }

				inline void Accum_Blue_Bits(std::int32_t accum_blue_bits = 0) { Set_Hint(*enums::framebuffer::Hints::Accum_Blue_Bits, accum_blue_bits); }

				inline void Accum_Alpha_Bits(std::int32_t accum_alpha_bits = 0) { Set_Hint(*enums::framebuffer::Hints::Accum_Alpha_Bits, accum_alpha_bits); }

				inline void AUX_Buffers(std::int32_t AUX_buffers = 0) { Set_Hint(*enums::framebuffer::Hints::AUX_Buffers, AUX_buffers); }

				inline void Stereo(bool stereo = false) { Set_Hint(*enums::framebuffer::Hints::Stereo, stereo); }

				inline void Samples(std::int32_t samples = 0) { Set_Hint(*enums::framebuffer::Hints::Samples, samples); }

				inline void SRGB_Capable(bool SRGB_capable = false) { Set_Hint(*enums::framebuffer::Hints::SRGB_Capable, SRGB_capable); }

				inline void Doublebuffer(bool doublebuffer = true) { Set_Hint(*enums::framebuffer::Hints::Doublebuffer, doublebuffer); }

			} // framebuffer

			namespace context
			{

				inline void Client_API(enums::context::Client_API client_API = enums::context::Client_API::OpenGL) { Set_Hint(*enums::context::Hints::Client_API, *client_API); }

				inline void Creation_API(enums::context::Creation_API context_creation_API = enums::context::Creation_API::Native) { Set_Hint(*enums::context::Hints::Creation_API, *context_creation_API); }

				inline void Verion_Major(std::int32_t context_verion_major = 1) { Set_Hint(*enums::context::Hints::Verion_Major, context_verion_major); }

				inline void Verion_Minor(std::int32_t context_verion_minor = 0) { Set_Hint(*enums::context::Hints::Verion_Minor, context_verion_minor); }

				inline void OpenGL_Forward_Compat(bool openGL_forward_compat = false) { Set_Hint(*enums::context::Hints::OpenGL_Forward_Compat, openGL_forward_compat); }

				inline void OpenGL_Debug_Context(bool openGL_debug_context = false) { Set_Hint(*enums::context::Hints::OpenGL_Debug_Context, openGL_debug_context); }

				inline void OpenGL_Profile(enums::context::OpenGL_Profile openGL_profile = enums::context::OpenGL_Profile::Any) { Set_Hint(*enums::context::Hints::OpenGL_Profile, *openGL_profile); }

				inline void Robustness(enums::context::Robustness context_robustness = enums::context::Robustness::No_Robustness) { Set_Hint(*enums::context::Hints::Robustness, *context_robustness); }

				inline void Release_Behavior(enums::context::Release_Behavior context_release_behavior = enums::context::Release_Behavior::Any) { Set_Hint(*enums::context::Hints::Release_Behavior, *context_release_behavior); }

			} // context

		} // hints

		namespace callbacks
		{

			template<enums::Callbacks Func_Type, typename Func_Parameters>
			struct Window_Func
			{
				using Parameters = Func_Parameters;
				using Signature = std::function<Parameters>;
				static constexpr auto Type{ Func_Type };
				Signature function{};
				Window_Func(Signature func) : function{ func } { }
			}; // Window_Func

			using Position = Window_Func<enums::Callbacks::Position,
				void(GLFWwindow*, std::int32_t, std::int32_t)>;

			using Size = Window_Func<enums::Callbacks::Size,
				void(GLFWwindow*, std::int32_t, std::int32_t)>;

			using Close = Window_Func<enums::Callbacks::Close,
				void(GLFWwindow*)>;

			using Refresh = Window_Func<enums::Callbacks::Refresh,
				void(GLFWwindow*)>;

			using Focus = Window_Func<enums::Callbacks::Focus,
				void(GLFWwindow*, std::int32_t)>;

			using Inconify = Window_Func<enums::Callbacks::Iconify,
				void(GLFWwindow*, std::int32_t)>;

			using Maximize = Window_Func<enums::Callbacks::Maximize,
				void(GLFWwindow*, std::int32_t)>;

			using Framebuffer_Size = Window_Func<enums::Callbacks::Framebuffer_Size,
				void(GLFWwindow*, std::int32_t, std::int32_t)>;

			using Content_Scale = Window_Func<enums::Callbacks::Content_Scale,
				void(GLFWwindow*, float, float)>;

		} // callbacks

		namespace properties
		{

			template<enums::Properties Type>
			struct Window_Property {};

			template<>
			struct Window_Property<enums::Properties::Should_Close>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Size>
			{
				using Set_Parameters = std::pair<std::int32_t, std::int32_t>;
				using Get_Parameters = std::pair<std::int32_t, std::int32_t>;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Frame_Size>
			{
				using Get_Parameters = std::tuple<std::int32_t, std::int32_t, std::int32_t, std::int32_t>;
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Framebuffer_Size>
			{
				using Get_Parameters = std::pair<std::int32_t, std::int32_t>;
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Content_Scale>
			{
				using Get_Parameters = std::pair<float, float>;
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Size_Limits>
			{
				using Set_Parameters = std::tuple<std::int32_t, std::int32_t, std::int32_t, std::int32_t>;
				static void Set(Window& window, Set_Parameters&& parameters);
			};

			template<>
			struct Window_Property<enums::Properties::Aspect_Ratio>
			{
				using Set_Parameters = std::pair<std::int32_t, std::int32_t>;
				static void Set(Window& window, Set_Parameters&& parameters);
			};

			template<>
			struct Window_Property<enums::Properties::Position>
			{
				using Set_Parameters = std::pair<std::int32_t, std::int32_t>;
				using Get_Parameters = std::pair<std::int32_t, std::int32_t>;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Title>
			{
				using Set_Parameters = std::string;
				using Get_Parameters = std::string;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			// Todo: When image loading is done, implement this
			template<>
			struct Window_Property<enums::Properties::Icon>
			{
			};

			template<>
			struct Window_Property<enums::Properties::Monitor>
			{
				using Set_Parameters = monitoring::Monitor;
				using Get_Parameters = monitoring::Monitor;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Fullscreen>
			{
				using Set_Parameters = std::tuple<monitoring::Monitor, std::pair<std::uint32_t, std::uint32_t>, std::uint32_t>;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Iconify>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Maximize>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Visible>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Focus>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Request_Attention>
			{
				static void Set(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Opacity>
			{
				using Set_Parameters = float;
				using Get_Parameters = float;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Swap_Interval>
			{
				using Set_Parameters = std::int32_t;
				using Get_Parameters = std::int32_t;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Resizable>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Decorated>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Auto_Iconify>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Floating>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Focus_On_Show>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Properties::Transparent_Framebuffer>
			{
				using Get_Parameters = bool;
				static Get_Parameters Get(Window& window);
			};

			// TODO: implement when framebuffer abstraction is implemented.
			template<>
			struct Window_Property<enums::Properties::Framebuffer>
			{

			};

			namespace context
			{

				template<enums::context::Properties Type>
				struct Context_Property {};

				template<>
				struct Context_Property<enums::context::Properties::Client_API>
				{
					using Get_Parameters = enums::context::Client_API;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::context::Properties::Creation_API>
				{
					using Get_Parameters = enums::context::Creation_API;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::context::Properties::Version>
				{
					using Get_Parameters = std::tuple<std::uint32_t, std::uint32_t, std::uint32_t>;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::context::Properties::OpenGL_Forward_Compatible>
				{
					using Get_Parameters = bool;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::context::Properties::OpenGL_Debug_Context>
				{
					using Get_Parameters = bool;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::context::Properties::OpenGL_Profile>
				{
					using Get_Parameters = enums::context::OpenGL_Profile;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::context::Properties::Release_Behavior>
				{
					using Get_Parameters = enums::context::Release_Behavior;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::context::Properties::No_Error>
				{
					using Get_Parameters = bool;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::context::Properties::Robustness>
				{
					using Get_Parameters = enums::context::Robustness;
					static Get_Parameters Get(Window& window);
				};

			} // context

		} // properties

	} // windowing

} // tilia

#endif // TILIA_WINDOW_PROPERTIES_HPP
