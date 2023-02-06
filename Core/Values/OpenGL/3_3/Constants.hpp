/**************************************************************************************************
 * @file   Constants.hpp
 * @brief  A header file holding enumerations for the TiliaEnigne as well as a function which
 * returns the underlying value of the given enum
 * 
 * @author Gustav Fagerlind
 * @date   28/05/2022
 *************************************************************************************************/

#ifndef TILIA_OPENGL_3_3_CONSTANTS_HPP
#define TILIA_OPENGL_3_3_CONSTANTS_HPP

// Standard
#include <cstdint>
#include <type_traits>

namespace tilia {

	namespace enums {

		// Different types of openGL primitives. Underlying value is the value defined by openGL.
		enum class Primitive {
			// Draws every vertex as a separate point.
			Points		   = 0x0000,
			// Connects two vertices to create a line.
			Lines		   = 0x0001,
			// Will connect any amount of vertices into a line that will loop back to first vertex.
			Line_Loop	   = 0x0002, 
			// Will connect any amount of vertices into a line.
			Line_Strip	   = 0x0003, 
			// Will connect three vertices into a triangle.
			Triangles      = 0x0004, 
			// Will draw connected triangles.
			Triangle_Strip = 0x0005, 
			// Will draw connected triangles in a fan shape.
			Triangle_Fan   = 0x0006  
		}; // Primitive

		// Different types of openGL polygon modes. Underlying value is the value defined by
		// openGL.
		enum class Polymode {
			// Will draw points.
			Point = 0x1B00, 
			// Will draw lines.
			Line  = 0x1B01, 
			// Will draw filled.
			Fill  = 0x1B02  
		}; // Polymode

		// Different types of color formats. Underlying value is the value defined by openGL.
		enum class Data_Color_Format {
			// None.
			None = 0x0000, 
			// The red color channel.
			Red  = 0x1903, 
			// The red, green, and blue color channels.
			RGB  = 0x1907, 
			// The red, green, and blue color channels and the alpha channel.
			RGBA = 0x1908, 
		}; // Data_Color_Format

		// Different types of color formats used internally by openGL. Underlying value is the
		// value defined by openGL.
		enum class Color_Format {
			// None.
			None  = 0x0000,	
			// The red color channel, consists of 8 bits(1 byte).
			Red8  = 0x8229, 
			// The red, green, and blue color channels, consists of 24 bits(3 bytes).
			RGB8  = 0x8051, 
			// The red, green, and blue color channels and the alpha channel, consists of 32 
			// bits(4 bytes).
			RGBA8 = 0x8058, 
		}; // Color_Format

		// Different types of filtering modes for textures. Underlying value is the value defined
		// by openGL.
		enum class Filter_Mode {
			// Samples from the nearest texel. Also called nearest. 
			Point     = 0x2600,   
			// Samples from all the nearest texels using linear interpolation. Also called linear. 
			Bilinear  = 0x2601    
			// Linearily interpolates between the nearest mipmaps and nearest texels. Also called
			// linear_mip.
			//Trilinear = 0x2703  
		}; // Filter_Mode

		// The different sizes that determine what way the texture should be filtered. Underlying
		// value is the value defined by openGL.
		enum class Filter_Size {
			// If the texture is magnified then the filtering on the texture will be that bound
			// with this size.
			Magnify  = 0x2800, 
			// If the texture is minified then the filtering on the texture will be that bound with
			// this size.
			Minify   = 0x2801  
		}; // Filter_Size

		// Different types of wrapping modes for textures if uv coordinates go outside of 0.0 and
		// 1.0. Underlying value is the value defined by openGL.
		enum class Wrap_Mode {
			// Repeats texture.
			Repeat        = 0x2901, 
			// Repeats texture but mirrors every other time.
			Mirror_Repeat = 0x8370, 
			// Clamps the texture to the edge of the shape draw.
			Clamp_Edge    = 0x812F  
		}; // Wrap_Mode

		// The wrapping sides for setting wrapping parameters. Underlying value is the value
		// defined by openGL.
		enum class Wrap_Sides {
			// The x-axis for wrapping.
			S, 
			// The y-axis axis for wrapping.
			T, 
			// The z-axis axis for wrapping.
			R  
		}; // Wrap_Sides

		// Different types of openGL textures. Underlying value is the value defined by openGL.
		enum class Texture_Type {
			// A 1-dimensional texture. Access with one value.
			OneD	 = 0x0DE0, 
			// A 2-dimensional texture. Access with two values.
			TwoD	 = 0x0DE1, 
			// A texture made up of six 2-dimensional textures. Access with three values.
			Cube_Map = 0x8513  
		}; // Texture_Type

		// The sides of the cube map. Underlying value is the value defined by openGL.
		enum class Cube_Map_Sides {
			// Positive side of the x-axis. 
			Positive_X = 0x8515, 
			// Negative side of the x-axis. 
			Negative_X = 0x8516, 
			// Positive side of the y-axis. 
			Positive_Y = 0x8517, 
			// Negative side of the y-axis. 
			Negative_Y = 0x8518, 
			// Positive side of the z-axis. 
			Positive_Z = 0x8519, 
			// Negative side of the z-axis. 
			Negative_Z = 0x851A  
		}; // Cube_Map_Sides

		// The faces/sides of a drawn surface. Underlying value is the value defined by openGL.
		enum class Face {
			// No face.
			None  = 0x0000, 
			// Front-facing faces.
			Front = 0x0404, 
			// Back-facing faces.
			Back  = 0x0405, 
			// Both back- and front-facing faces.
			Both  = 0x0408  
		}; // Face

		// The order openGL interprets the vertex-data which determines what faces are front
		// facing. Underlying value is the value defined by openGL.
		enum class Winding_Order {
			CW  = 0x0900, // Clock-wise.
			CCW = 0x0901  // Counter-clock-wise.
		}; // Winding_Order

		// The different options for the openGL (eg. depth) test functions. Underlying value is the
		// value defined by openGL.
		enum class Test_Func {
			// No testing
			None     = 0x0000, 
			// When used, test never succeeds.
			Never	 = 0x0200, 
			// When used, test succeeds when data is less than previous data.
			Less	 = 0x0201, 
			// When used, test succeeds when data is equal to the previous data.
			Equal	 = 0x0202, 
			// When used, test succeeds when is less than or equal to the previous data.
			Lequal   = 0x0203, 
			// When used, test succeeds when data is greater than previous data.
			Greater  = 0x0204, 
			// When used, test succeeds when data is not equal to the previous data.
			Notequal = 0x0205, 
			// When used, test succeeds when is greater than or equal to the previous data.
			Gequal   = 0x0206, 
			// When used, test always succeeds.
			Always   = 0x0207  
		}; // Test_Func

		// The different actions to take wheter different tests passes or fails to change the
		// stencil buffer. Underlying value is the value defined by openGL.
		enum class Test_Action {
			// Keeps the previously stored value.
			Keep	  = 0x1E00, 
			// Sets value to zero.
			Zero	  = 0x0000, 
			// Replaces value with the previously set reference value.
			Replace	  = 0x1E01, 
			// Increases the previosly stored value by one if the value is less than max value.
			Incr	  = 0x1E02, 
			// Increases the previosly stored value by one and wraps to zero.
			Incr_Wrap = 0x8507, 
			// Decreases the previosly stored value by one if the value is more than zero.
			Decr      = 0x1E03, 
			// Increases the previosly stored value by one and wraps to max value.
			Decr_Wrap = 0x8508, 
			// Bitwise inverts the previosly stored value.
			Invert    = 0x150A  
		}; // Test_Action

		// Different values used for blending. Underlying value is the value defined by openGL.
		enum class Blend_Func  {
			// No blending.
			None					 = 0x0000, 
			// Blending factor is zero.
			Zero					 = 0x0000, 
			// Blending factor is one.
			One						 = 0x0001, 
			// Blending factor is equal to the source color.
			Src_Color				 = 0x0300, 
			// Blending factor is equal to one minus the source color.
			One_Minus_Src_Color		 = 0x0301, 
			// Blending factor is equal to the destination color.
			Dst_Color				 = 0x0306, 
			// Blending factor is equal to one minus the destination color.
			One_Minus_Dst_Color		 = 0x0307, 
			// Blending factor is equal to the source alpha.
			Src_Alpha				 = 0x0302, 
			// Blending factor is equal to one minus the source alpha.
			One_Minus_Src_Alpha		 = 0x0303, 
			// Blending factor is equal to the destination alpha.
			Dst_Alpha				 = 0x0304, 
			// Blending factor is equal to one minus the destination alpha.
			One_Minus_Dst_Alpha		 = 0x0305, 
			// Blending factor is equal to the constant color.
			Constant_Color			 = 0x8001, 
			// Blending factor is equal to one minus the constant color.
			One_Minus_Constant_Color = 0x8002, 
			// Blending factor is equal to the constant alpha.
			Constant_Alpha			 = 0x8003, 
			// Blending factor is equal to one minus the constant alpha.
			One_Minus_Constant_Alpha = 0x8004  
		}; // Blend_Func

		// Different types of openGL shaders. Underlying value is the value defined by openGL.
		enum class Shader_Type {
			// The fragment shader. Also known as pixel shader.
			Fragment  = 0x8B30, 
			// The vertex shader.
			Vertex	  = 0x8B31, 
			// The geometry shader.
			Geometry  = 0x8DD9  
		}; // Shader_Type

		// GLSL data scalar types. Underlying value is the value defined by openGL.
		enum class GLSL_Scalar_Type {
			// A boolean.
			Bool   = 0x8B56, 
			// An integer.
			Int    = 0x1404, 
			// An unisgned integer.
			Uint   = 0x1405, 
			// A single precision floating point number
			Float  = 0x1406  
		}; // GLSL_Scalar_Type

		// GLSL container types. Value is the amount of scalars.
		// https://www.oreilly.com/library/view/opengl-programming-guide/9780132748445/app09lev1sec2.html
		enum class GLSL_Container_Type {
			// Eg. bool, int, float...
			Scalar    = 1,           
			// Eg. vec2, ivec2, bvec2...
			Vector2   = 2,           
			// Eg. vec3, ivec3, bvec3...
			Vector3   = 3,           
			// Eg. vec4, ivec4, bvec4...
			Vector4   = 4,           
			// mat2, mat2x3, mat2x4.
			Matrix2   = 2 * Vector4, 
			// mat3, mat3x2, mat3x4.
			Matrix3   = 3 * Vector4, 
			// mat4, mat4x3, mat4x2.
			Matrix4   = 4 * Vector4, 
		}; // GLSL_Container_Type

		// OpenGL buffer types. Underlying value is the value defined by openGL.
		enum class Buffer_Type {
			// Vertex buffer object, vbo.
			Vertex_Buffer  = 0x8892, 
			// Element buffer object, ebo.
		    Element_Buffer = 0x8893, 
			// Uniform buffer object, ubo.
			Uniform_Buffer = 0x8A11  
		}; // Buffer_Type

		// Different ways to access openGL buffers. Add to buffer frequency value to combine into
		// an openGL enum value for that type of writing. Eg. Static + Draw = GL_STATIC_DRAW
		// (0x88E4).
		enum class Buffer_Access_Type {
			// The user will be writing data to the buffer, but the user will not read it.
			Draw = 0x0000, 
			// The user will not be writing data, but the user will be reading it back.
			Read = 0x0001, 
			// The user will be neither writing nor reading the data. 
			Copy = 0x0002  
		}; // Buffer_Access_Type

		// Different frequencies to access openGL buffers. Underlying value is the value defined by
		// openGL.
		enum class Buffer_Access_Frequency {
			// The user will set the data once.
			Static  = 0x88E4, 
			// The user will set the data occasionally.
			Dynamic = 0x88E8, 
			// The user will be changing the data after every use. Or almost every use.
			Stream  = 0x88E0  
		}; // Buffer_Access_Frequency

		// Different ways to map to openGL buffers. Underlying value is the value defined by
		// openGL.
		enum class Buffer_Map_Type {
			// The user is not mapped.
			None	   = 0x0000,
			// The user will be mapped to the local buffer.
			Local	   = 0x0000,
			// The user will only be able to read from the mapped memory.
			Read_Only  = 0x88B8, 
			// The user will only be able to write to the mapped memory.
			Write_Only = 0x88B9, 
			// The user will be able to both read from and write to the mapped memory.
			Read_Write = 0x88BA  
		}; // Buffer_Map_Type

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
			return static_cast<std::underlying_type_t<E>>(e);
		}

#endif // TILIA_GET_ENUM_VALUE

	} // enums

} // tilia

#endif // TILIA_OPENGL_3_3_CONSTANTS_HPP