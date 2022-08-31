/*****************************************************************//**
 * @file   Enums.h
 * @brief  A header file holding enumerations for the TiliaEnigne as well as a function which returns the underlying value of the given enum
 * 
 * Standard:
 * @include <cstdint>
 * @include <type_traits>
 * 
 * @author Gustav Fagerlind
 * @date   28/05/2022
 *********************************************************************/

#ifndef TILIA_ENUMS_H
#define TILIA_ENUMS_H

// Standard
#include <cstdint>
#include <type_traits>

namespace tilia {

	namespace enums {

		// Different types of openGL primitives. Underlying value is the value defined by openGL.
		enum class Primitive {
			Points		   = 0x0000, // Draws every vertex as a separate point.
			Lines		   = 0x0001, // Connects two vertices to create a line.
			Line_Loop	   = 0x0002, // Will connect any amount of vertices into a line that will loop back to first vertex.
			Line_Strip	   = 0x0003, // Will connect any amount of vertices into a line.
			Triangles      = 0x0004, // Will connect three vertices into a triangle.
			Triangle_Strip = 0x0005, // Will draw connected triangles.
			Triangle_Fan   = 0x0006  // Will draw connected triangles in a fan shape.
		};

		// Different types of openGL polygon modes. Underlying value is the value defined by openGL.
		enum class Polymode {
			Point = 0x1B00, // Will draw points.
			Line  = 0x1B01, // Will draw lines.
			Fill  = 0x1B02  // Will draw filled.
		};

		// Different types of color formats. Underlying value is the value defined by openGL.
		enum class Data_Color_Format {
			None = 0x0000,	// None.
			Red  = 0x1903, // The red color channel.
			RGB  = 0x1907, // The red, green, and blue color channels.
			RGBA = 0x1908, // The red, green, and blue color channels and the alpha channel.
		};

		// Different types of color formats used internally by openGL. Underlying value is the value defined by openGL.
		enum class Color_Format {
			None  = 0x0000,	// None.
			Red8  = 0x8229, // The red color channel, consists of 8 bits(1 byte).
			RGB8  = 0x8051, // The red, green, and blue color channels, consists of 24 bits(3 bytes).
			RGBA8 = 0x8058, // The red, green, and blue color channels and the alpha channel, consists of 32 bits(4 bytes).
		};

		// Different types of filtering modes for textures. Underlying value is the value defined by openGL.
		enum class Filter_Mode {
			Point     = 0x2600,   // Samples from the nearest texel. Also called nearest. 
			Bilinear  = 0x2601    // Samples from all the nearest texels using linear interpolation. Also called linear. 
			//Trilinear = 0x2703  // Linearily interpolates between the nearest mipmaps and nearest texels. Also called linear_mip.
		};

		// The different sizes that determine what way the texture should be filtered. Underlying value is the value defined by openGL.
		enum class Filter_Size {
			Magnify  = 0x2800, // If the texture is magnified then the filtering on the texture will be that bound with this size.
			Minify   = 0x2801  // If the texture is minified then the filtering on the texture will be that bound with this size.
		};

		// Different types of wrapping modes for textures if uv coordinates go outside of 0.0 and 1.0. Underlying value is the value defined by openGL.
		enum class Wrap_Mode {
			Repeat        = 0x2901, // Repeats texture.
			Mirror_Repeat = 0x8370, // Repeats texture but mirrors every other time.
			Clamp_Edge    = 0x812F  // Clamps the texture to the edge of the shape draw.
		};

		// The wrapping sides for setting wrapping parameters. Underlying value is the value defined by openGL.
		enum class Wrap_Sides {
			S, // The x-axis for wrapping.
			T, // The y-axis axis for wrapping.
			R  // The z-axis axis for wrapping.
		};

		// Different values used for certain things.
		enum class Misc {
			Max_Triangles	= 16384,		     // The max amount of triangles per batch.
			Max_Vertices	= Max_Triangles * 3, // The max amount of vertices per batch.
			Max_Indices		= Max_Vertices * 2,  // The max amount of indices per batch.
			Cube_Sides		= 6,				 // The amount of sides a cube has.
			Point_Points	= 1,				 // The amount of points in a point.
			Line_Points     = 2,				 // The amount of points in a line.
			Triangle_Points = 3,				 // The amount of points in a triangle.
			Quad_Points     = 4 				 // The amount of points in a quad.
		};

		// Different types of openGL textures. Underlying value is the value defined by openGL.
		enum class Texture_Type {
			OneD	 = 0x0DE0, // A 1-dimensional texture. Access with one value.
			TwoD	 = 0x0DE1, // A 2-dimensional texture. Access with two values.
			Cube_Map = 0x8513  // A texture made up of six 2-dimensional textures. Access with three values.
		};

		// The sides of the cube map. Underlying value is the value defined by openGL.
		enum class Cube_Map_Sides {
			Positive_X = 0x8515, // Positive side of the x-axis. 
			Negative_X = 0x8516, // Negative side of the x-axis. 
			Positive_Y = 0x8517, // Positive side of the y-axis. 
			Negative_Y = 0x8518, // Negative side of the y-axis. 
			Positive_Z = 0x8519, // Positive side of the z-axis. 
			Negative_Z = 0x851A  // Negative side of the z-axis. 
		};

		// The faces/sides of a drawn surface. Underlying value is the value defined by openGL.
		enum class Face {
			None  = 0x0000, // No face.
			Front = 0x0404, // Front-facing faces.
			Back  = 0x0405, // Back-facing faces.
			Both  = 0x0408  // Both back- and front-facing faces.
		};

		// The order openGL interprets the vertex-data which determines what faces are front facing. Underlying value is the value defined by openGL.
		enum class Winding_Order {
			CW  = 0x0900, // Clock-wise.
			CCW = 0x0901  // Counter-clock-wise.
		};

		// The different options for the openGL (eg. depth) test functions. Underlying value is the value defined by openGL.
		enum class Test_Func {
			None     = 0x0000, // No testing
			Never	 = 0x0200, // When used, test never succeeds.
			Less	 = 0x0201, // When used, test succeeds when data is less than previous data.
			Equal	 = 0x0202, // When used, test succeeds when data is equal to the previous data.
			Lequal   = 0x0203, // When used, test succeeds when is less than or equal to the previous data.
			Greater  = 0x0204, // When used, test succeeds when data is greater than previous data.
			Notequal = 0x0205, // When used, test succeeds when data is not equal to the previous data.
			Gequal   = 0x0206, // When used, test succeeds when is greater than or equal to the previous data.
			Always   = 0x0207  // When used, test always succeeds.
		};

		// The different actions to take wheter different tests passes or fails to change the stencil buffer. Underlying value is the value defined by openGL.
		enum class Test_Action {
			Keep	  = 0x1E00, // Keeps the previously stored value.
			Zero	  = 0x0000, // Sets value to zero.
			Replace	  = 0x1E01, // Replaces value with the previously set reference value.
			Incr	  = 0x1E02, // Increases the previosly stored value by one if the value is less than max value.
			Incr_Wrap = 0x8507, // Increases the previosly stored value by one and wraps to zero.
			Decr      = 0x1E03, // Decreases the previosly stored value by one if the value is more than zero.
			Decr_Wrap = 0x8508, // Increases the previosly stored value by one and wraps to max value.
			Invert    = 0x150A  // Bitwise inverts the previosly stored value.
		};

		// Different values used for blending. Underlying value is the value defined by openGL.
		enum class Blend_Func  {
			None					 = 0x0000, // No blending.
			Zero					 = 0x0000, // Blending factor is zero.
			One						 = 0x0001, // Blending factor is one.
			Src_Color				 = 0x0300, // Blending factor is equal to the source color.
			One_Minus_Src_Color		 = 0x0301, // Blending factor is equal to one minus the source color.
			Dst_Color				 = 0x0306, // Blending factor is equal to the destination color.
			One_Minus_Dst_Color		 = 0x0307, // Blending factor is equal to one minus the destination color.
			Src_Alpha				 = 0x0302, // Blending factor is equal to the source alpha.
			One_Minus_Src_Alpha		 = 0x0303, // Blending factor is equal to one minus the source alpha.
			Dst_Alpha				 = 0x0304, // Blending factor is equal to the destination alpha.
			One_Minus_Dst_Alpha		 = 0x0305, // Blending factor is equal to one minus the destination alpha.
			Constant_Color			 = 0x8001, // Blending factor is equal to the constant color.
			One_Minus_Constant_Color = 0x8002, // Blending factor is equal to one minus the constant color.
			Constant_Alpha			 = 0x8003, // Blending factor is equal to the constant alpha.
			One_Minus_Constant_Alpha = 0x8004  // Blending factor is equal to one minus the constant alpha.
		};

		/**
		 * @brief Can take any enum class as parameter and will return, if there is any the underlying value.
		 *
		 * @param e - The enum class value
		 *
		 * @return The underlying value of the enum class value
		 */
		template<typename E>
		constexpr typename std::underlying_type<E>::type Get_GL_Value(E e) noexcept {
			return static_cast<typename std::underlying_type<E>::type>(e);
		}

		/**
		 * @brief Template operator for the enumerators.
		 * 
		 * @param e - The enumerator value the get the value of.
		 * 
		 * @return The value of the given enumerator value.
		 */
		template<typename E>
		constexpr auto operator*(const E& e) {
			return Get_GL_Value(e);
		}

	}

}

#endif