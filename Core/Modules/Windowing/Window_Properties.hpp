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

			template<>
			struct Window_Property<enums::Window_Properties::Monitor>
			{
				using Set_Parameters = monitoring::Monitor;
				using Get_Parameters = monitoring::Monitor;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Iconify>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Maximize>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Visible>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Focus>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Request_Attention>
			{
				using Set_Parameters = void;
				static void Set(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Opacity>
			{
				using Set_Parameters = float;
				using Get_Parameters = float;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Swap_Interval>
			{
				using Set_Parameters = std::int32_t;
				using Get_Parameters = std::int32_t;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Resizable>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Decorated>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Auto_Iconify>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Floating>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Focus_On_Show>
			{
				using Set_Parameters = bool;
				using Get_Parameters = bool;
				static void Set(Window& window, Set_Parameters&& parameters);
				static Get_Parameters Get(Window& window);
			};

			template<>
			struct Window_Property<enums::Window_Properties::Transparent_Framebuffer>
			{
				using Get_Parameters = bool;
				static Get_Parameters Get(Window& window);
			};

			namespace context
			{

				template<enums::Context_Properties Type>
				struct Context_Property {};

				template<>
				struct Context_Property<enums::Context_Properties::Client_API>
				{
					using Get_Parameters = enums::Client_API;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::Context_Properties::Creation_API>
				{
					using Get_Parameters = enums::Context_Creation_API;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::Context_Properties::Version>
				{
					using Get_Parameters = std::tuple<std::uint32_t, std::uint32_t, std::uint32_t>;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::Context_Properties::OpenGL_Forward_Compatible>
				{
					using Get_Parameters = bool;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::Context_Properties::OpenGL_Debug_Context>
				{
					using Get_Parameters = bool;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::Context_Properties::OpenGL_Profile>
				{
					using Get_Parameters = enums::OpenGL_Profile;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::Context_Properties::Release_Behavior>
				{
					using Get_Parameters = enums::Context_Release_Behavior;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::Context_Properties::No_Error>
				{
					using Get_Parameters = bool;
					static Get_Parameters Get(Window& window);
				};

				template<>
				struct Context_Property<enums::Context_Properties::Robustness>
				{
					using Get_Parameters = enums::Context_Robustness;
					static Get_Parameters Get(Window& window);
				};

			} // context

		} // properties

	} // windowing

} // tilia

#endif // TILIA_WINDOW_PROPERTIES_HPP
