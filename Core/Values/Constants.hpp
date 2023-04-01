/**************************************************************************************************
 * @file   Constants.hpp
 * @brief  Different constant values which are used throughout the Tilia project.
 *
 * @author Gustav Fagerlind
 * @date   07/01/2023
 *************************************************************************************************/

#ifndef TILIA_CONSTANTS_HPP
#define TILIA_CONSTANTS_HPP

 // Standard
#include <cstdint>
#include <type_traits>

// Tilia
#include "Core/Values/Directories.hpp"

/**
 * @brief If 'TILIA_INCLUDE_OPENGL_3_3_ENUMS' is defined then we include the openGL 3.3 Tilia
 * constants.
 */
#if defined(TILIA_INCLUDE_OPENGL_3_3_CONSTANTS) && !defined(TILIA_OPENGL_3_3_CONSTANTS_HPP)

#include TILIA_OPENGL_3_3_CONSTANTS_INCLUDE

#endif // TILIA_OPENGL_3_3_ENUMS

namespace tilia
{
    namespace enums
    {

		// Limiting constants of Tilia batch.
		enum class Batch_Limits {
			Max_Triangles = 16384,		        // The max amount of triangles per batch.
			Max_Vertices  = Max_Triangles * 3,  // The max amount of vertices per batch.
			Max_Indices   = Max_Vertices * 2    // The max amount of indices per batch.
		}; // Batch_Limits

		// Different constants for features of different geometrical shapes.
		enum class Geometry_Features {
			Cube_Faces      = 6, // The amount of faces a cube has.
			Point_Points    = 1, // The amount of points in a point.
			Line_Points     = 2, // The amount of points in a line.
			Triangle_Points = 3, // The amount of points in a triangle.
			Quad_Points     = 4  // The amount of points in a quad.
		}; // Geometry_Features

		// Window hints which can be used to set different values used in creating windows.
		// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
		enum class Window_Hints {
			Resizable				 = 0x00020003,
			Visible					 = 0x00020004,
			Decorated				 = 0x00020005,
			Focused					 = 0x00020001,
			Auto_Iconify			 = 0x00020006,
			Floating				 = 0x00020007,
			Maximized				 = 0x00020008,
			Center_Cursor			 = 0x00020009,
			Transparent_Framebuffer  = 0x0002000A,
			Focus_On_Show			 = 0x0002000C,
			Scale_To_Monitor		 = 0x0002200C,
			Refresh_Rate			 = 0x0002100F
		}; // Window_Hints

		// Framebuffer hints which can be used to set different values used in creating windows.
		// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
		enum class Framebuffer_Hints
		{
			Red_Bits		 = 0x00021001,
			Green_Bits		 = 0x00021002,
			Blue_Bits		 = 0x00021003,
			Alpha_Bits		 = 0x00021004,
			Depth_Bits		 = 0x00021005,
			Stencil_Bits	 = 0x00021006,
			Accum_Red_Bits   = 0x00021007,
			Accum_Green_Bits = 0x00021008,
			Accum_Blue_Bits  = 0x00021009,
			Accum_Alpha_Bits = 0x0002100A,
			AUX_Buffers		 = 0x0002100B,
			Stereo			 = 0x0002100C,
			Samples			 = 0x0002100D,
			SRGB_Capable	 = 0x0002100E,
			Doublebuffer	 = 0x00021010,
		}; // Framebuffer_Hints

		// Contexts hints which can be used to set different values used in creating windows.
		// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
		enum class Context_Hints
		{
			Client_API			  = 0x00022001,
			Creation_API		  = 0x0002200B,
			Verion_Major		  = 0x00022002,
			Verion_Minor		  = 0x00022003,
			OpenGL_Forward_Compat = 0x00022006,
			OpenGL_Debug_Context  = 0x00022007,
			OpenGL_Profile		  = 0x00022008,
			Robustness			  = 0x00022005,
			Release_Behavior	  = 0x00022009,
		}; // Context_Hints

		// Windowing callbacks.
		// Check this out for info: https://www.glfw.org/docs/3.3/group__window.html
		enum class Window_Callbacks
		{
			Position		 = 0x0000,
			Size			 = 0x0001,
			Close			 = 0x0002,
			Refresh			 = 0x0003,
			Focus			 = 0x0004,
			Iconify			 = 0x0005,
			Maximize		 = 0x0006,
			Framebuffer_Size = 0x0007,
			Content_Scale    = 0x0008
		}; // Window_Callbacks

		// Window properties.
		// Check this out for info: https://www.glfw.org/docs/3.3/window_guide.html
		enum class Window_Properties
		{
			Underlying_Window		= 0x0000,
			Should_Close			= 0x0001,
			Size					= 0x0002,
			Frame_Size				= 0x0003,
			Framebuffer_Size		= 0x0004,
			Content_Scale			= 0x0005,
			Size_Limits				= 0x0006,
			Aspect_Ratio			= 0x0007,
			Position				= 0x0008,
			Title					= 0x0009,
			Icon					= 0x000A,
			Monitor					= 0x000B,
			Iconify					= 0x000C,
			Maximize				= 0x000D,
			Visible					= 0x000E,
			Focus					= 0x000F,
			Request_Attention		= 0x0010,
			Opacity					= 0x0011,
			Swap_Interval			= 0x0012,
			Resizable				= 0x0013,
			Decorated				= 0x0014,
			Auto_Iconify			= 0x0015,
			Floating				= 0x0016,
			Focus_On_Show			= 0x0017,
			Transparent_Framebuffer = 0x0018,
			Framebuffer				= 0x0019
		}; // Window_Properties

		// Context properties.
		// Check this out for info: https://www.glfw.org/docs/3.3/window_guide.html
		enum class Context_Properties
		{
			Client_API				  = 0x0000,
			Creation_API			  = 0x0001,
			Version					  = 0x0002,
			OpenGL_Forward_Compatible = 0x0003,
			OpenGL_Debug_Context	  = 0x0004,
			OpenGL_Profile			  = 0x0005,
			Release_Behavior          = 0x0006,
			No_Error				  = 0x0007,
			Robustness				  = 0x0008
		}; // Context_Properties

		// What graphics API to use.
		// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
		enum class Client_API
		{
			No_API	  = 0x00000000,
			OpenGL	  = 0x00030001,
			OpenGL_ES = 0x00030002
		}; // Client_API

		// What API to create contexts for openGL.
		// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
		enum class Context_Creation_API
		{
			Native = 0x00036001,
			EGL	   = 0x00036002,
			OSMesa = 0x00036003
		}; // Context_Creation_API

		// The robustness of the created context when creating a window.
		// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
		enum class Context_Robustness
		{
			No_Robustness		  = 0x00000000,
			No_Reset_Notification = 0x00031001,
			Lose_Context_On_Reset = 0x00031002
		}; // Context_Robustness

		// What happens when the context is released.
		// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
		enum class Context_Release_Behavior
		{
			Any   = 0x00000000,
			Flush = 0x00035001,
			None  = 0x00035002
		}; // Context_Release_Behavior

		// The profile of the openGL context.
		// Check this out for info: https://www.glfw.org/docs/latest/window_guide.html
		enum class OpenGL_Profile
		{
			Any    = 0x00000000,
			Core   = 0x00032001,
			Compat = 0x00032002
		}; // OpenGL_Profile

		enum class Monitor_Properties
		{
			Underlying_Monitor		= 0x0000,
			Resolution				= 0x0001,
			Bit_Depths				= 0x0002,
			Refresh_Rate			= 0x0003,
			Supported_Resolutions	= 0x0004,
			Supported_Bit_Depths    = 0x0005,
			Supported_Refresh_Rates = 0x0006,
			Physical_Size			= 0x0007,
			Content_Scale			= 0x0008,
			Virtual_Position		= 0x0009,
			Workarea				= 0x000A,
			Name					= 0x000B,
			Gamma_Ramp				= 0x000C,
			Gamma					= 0x000D
		}; // Monitor_Properties

#ifndef TILIA_GET_ENUM_VALUE
#define TILIA_GET_ENUM_VALUE

		/**
		 * @brief Can take any enum value as parameter and will return (if any) the underlying
		 * value.
		 *
		 * @param e - The enum value
		 *
		 * @return The underlying value of the enum value
		 */
		template<typename E>
		constexpr auto Get_Enum_Value(const E& e) noexcept {
			return static_cast<std::underlying_type_t<E>>(e);
		}

		/**
		 * @brief Can take any enum value as parameter and will return (if any) the underlying
		 * value.
		 *
		 * @param e - The enum value
		 *
		 * @return The underlying value of the enum value
		 */
		template<typename E>
		constexpr auto operator*(const E& e) noexcept {
			return Get_Enum_Value(e);
		}

#endif // TILIA_GET_ENUM_VALUE

    }// enums

	constexpr std::int32_t DO_NOT_CARE{ -1 };

} // tilia

/**
 * @brief Whether or not to include unit tests
 */
#define TILIA_UNIT_TESTS 1

#endif // TILIA_CONSTANTS_HPP
