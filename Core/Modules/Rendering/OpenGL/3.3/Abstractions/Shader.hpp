/**************************************************************************************************
 * @file   Shader.hpp
 * 
 * @brief  Declares and defines a class called Shader which works as an abstraction for an openGL
 *		   shader containing a vertex shader and a fragment shader.
 * 
 * @define TILIA_SHADER_HPP
 * 
 * Vendor:
 * "dependencies/glm/include/glm/glm.hpp"
 * 
 * Standard:
 * <unordered_map>
 * <string>
 * 
 * Tilia:
 * "Shader_Data.hpp"
 * 
 * @author Gustav Fagerlind
 * @date   15/05/2022
 *************************************************************************************************/

#ifndef TILIA_SHADER_HPP
#define TILIA_SHADER_HPP

// Vendor
#include "vendor/glm/include/glm/glm.hpp"

// Standard
#include <unordered_map>
#include <string>

// Tilia
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Shader_Data.hpp"

namespace tilia {

	namespace gfx {

		/**
		 * @brief A struct that holds information for the Shader class.
		 */
		struct Shader_Type {
			// The path to the file of the shader.
			std::string path{};
			// The source code of the shader.
			std::string source{};
		}; // Shader_Type

#if 0
		
		/**
		 * @brief A class which works as an abstraction for an openGL shader.
		 */
		class Shader {
		public:
			
			/**
			 * @brief Creates a new shader using the two Shader_Types given.
			 * 
			 * @param vert_shader - The vertex-shader to be added.
			 * @param frag_shader - The fragment-shader to be added.
			 * 
			 * @exception Returns true or false whether the creation of the shader was successful. 
			 */
			bool Init(const Shader_Type& vert_shader, const Shader_Type& frag_shader);
			/**
			 * @brief Creates a new shader by loading the two different types of shaders using the given
			 *		  paths.
			 * 
			 * @param vert_path - The path to the vertex-shader to be added.
			 * @param frag_path - The path to the fragment-shader to be added.
			 * 
			 * @exception Returns true or false whether the creation of the shader was successful. 
			 */
			bool Init(const std::string& vert_path, const std::string& frag_path);
			/**
			 * @brief The kind-of copy-constructor, except it generates a new shader for openGL.
			 *
			 * @param shader - The Shader for which to copy the values of.
			 * 
			 * @exception Returns true or false whether the creation of the shader was successful. 
			 */
			bool Init(const Shader& shader);

			/**
			 * @brief The default destructor which deletes the openGL shader of this id.
			 */
			~Shader();

			/**
			 * @brief Binds the shader
			 * 
			 * @exception If m_ID is zero, returns and prints error
			 */
			void Bind() const;

			/**
			 * @brief Unbinds the bound shader. If save_id param is true, then saves
			 * the previously bound shaders's id.
			 *
			 * @param save_id - If true stores previous shader id
			 */
			void Unbind(const bool& save_id = false);

			/**
			 * @brief Binds the stored previously bound shader id
			 */
			void Rebind() const;

			/**
			 * @brief Gets the Shader_Type of the given type
			 * 
			 * @param type - The type of the shader to be returned. 
			 * 0 is the vertex-shader and 1 is the fragment-shader. If type is anything 
			 * else than these options the vertex-shader is returned.
			 * 
			 * @return The Shader_Type of the type given
			 */
			inline const Shader_Type& Get_Shader(const uint8_t& type) const {
				switch (type)
				{
				case 0:
					return m_vertex_shader;
				case 1:
					return m_fragment_shader;
				default:
					return m_vertex_shader;
				}
			}

			/**
			 * @brief Returns the id of the openGL shader this class abstracts
			 *
			 * @return m_ID - The id of the openGL shader this class abstracts
			 */
			inline uint32_t Get_ID() const { return m_ID; }

			/**
			 * @brief Uniform functions used to set uniform variables in either vertex- or fragment-shader. 
			 * 
			 * @param location - The name of the uniform variable to be set
			 * @param wdw - Some parameters which to set the uniform variable to
			 */
			void Uniform(const std::string& location) {};

			// Sets floats

			void Uniform(const std::string& location, const float& v0);
			void Uniform(const std::string& location, const float& v0, const float& v1);
			void Uniform(const std::string& location, const float& v0, const float& v1, const float& v2);
			void Uniform(const std::string& location, const float& v0, const float& v1, const float& v2, const float& v3);

			// Sets signed integers

			void Uniform(const std::string& location, const int32_t& v0);
			void Uniform(const std::string& location, const int32_t& v0, const int32_t& v1);
			void Uniform(const std::string& location, const int32_t& v0, const int32_t& v1, const int32_t& v2);
			void Uniform(const std::string& location, const int32_t& v0, const int32_t& v1, const int32_t& v2, const int32_t& v3);

			// Sets unsigned integers

			void Uniform(const std::string& location, const uint32_t& v0);
			void Uniform(const std::string& location, const uint32_t& v0, const uint32_t& v1);
			void Uniform(const std::string& location, const uint32_t& v0, const uint32_t& v1, const uint32_t& v2);
			void Uniform(const std::string& location, const uint32_t& v0, const uint32_t& v1, const uint32_t& v2, const uint32_t& v3);

			// Sets float vectors

			void Uniform(const std::string& location, const glm::vec2& v0);
			void Uniform(const std::string& location, const glm::vec3& v0);
			void Uniform(const std::string& location, const glm::vec4& v0);

			// Sets signed integer vectors

			void Uniform(const std::string& location, const glm::ivec2& v0);
			void Uniform(const std::string& location, const glm::ivec3& v0);
			void Uniform(const std::string& location, const glm::ivec4& v0);

			// Sets unsigned integer vectors

			void Uniform(const std::string& location, const glm::uvec2& v0);
			void Uniform(const std::string& location, const glm::uvec3& v0);
			void Uniform(const std::string& location, const glm::uvec4& v0);

			// Sets float vector

			void Uniform(const std::string& location, const size_t& count, const float* v0);

			// Sets signed integer vector

			void Uniform(const std::string& location, const size_t& count, const int32_t* v0);

			// Sets unsigned integer vector

			void Uniform(const std::string& location, const size_t& count, const uint32_t* v0);

			// Sets same width and height matricies

			void Uniform(const std::string& location, const glm::mat2& v0);
			void Uniform(const std::string& location, const glm::mat3& v0);
			void Uniform(const std::string& location, const glm::mat4& v0);

			// Sets using Shader_Data

			void Uniform(const Shader_Data& shader_data);

		private:

			Shader_Type m_vertex_shader{};	 // The vertex-shader
			Shader_Type m_fragment_shader{}; // The fragment-shader

			static uint32_t s_bound_ID; // The stored currently bound shader id.

			static uint32_t s_previous_ID; // The stored perviously bound shader id.

			uint32_t m_ID{}; // The shader program id

			std::unordered_map<std::string, int32_t> m_uniform_locs{}; // Stores uniform locations for uniform names

			/**
			 * @brief Passes m_vertex_shader and m_fragment_shader to the Parse_Shader function.
			 * Then calls the Create_Shader function.
			 *
			 * @exception If the functions called return false, throws exception which prints error
			 */
			bool Generate_Shader();
			/**
			 * @brief Loads in the source code from the path of the Shader_Type
			 *
			 * @param shader - The Shader_Type for which to load the source code for
			 *
			 * @return Returns true if everything went fine and false if an exeption was thrown
			 *
			 * @exception The ifstream exceptions. If error is thrown, prints errors to the console and returns false
			 */
			bool Parse_Shader(Shader_Type& shader) const;
			/**
			 * @brief Creates, sets and compiles an openGL texture of type, type
			 *
			 * @param type   - The type of the shader to be compiled
			 * @param shader - The Shader_Type for which to load the source code for
			 * @param id     - The id of the created shader
			 *
			 * @return Returns true if everything went fine and false if something went wrong
			 *
			 * @exception If error is found, prints errors to console
			 */
			bool Compile_Shader(const uint32_t& type, Shader_Type& shader, int32_t& id);
			/**
			 * @brief Creates an openGL shader program, sets m_ID to generated id, compiles the two shaders
			 * and then attatches the two shaders to this shader program
			 *
			 * @return Returns true if everything went fine and false if the two shaders do not compile
			 */
			bool Create_Shader();
			/**
			 * @brief Gets the uniform location of the uniform name name and if not already, adds the
			 * location to m_uniform_locs with the key name
			 *
			 * @param name - The name of the uniform to get the location to
			 *
			 * @return The location of the uniform with name name
			 */
			int32_t Get_Uniform_Location(const std::string& name);

		};

#endif // 0
		
		class Shader {
		public:

			Shader(const Shader_Type& vertex, 
				const Shader_Type& fragment, 
				const Shader_Type& geometry = {});

			Shader(const std::string& vertex_path,
				const std::string& fragment_path,
				const std::string& geometry_path = "");

			void Reload(const Shader_Type& shader);
			void Reload(const std::string& shader_path);

			void Reload(const Shader_Type& vertex,
				const Shader_Type& fragment,
				const Shader_Type& geometry = {});

			void Reload(const std::string& vertex_path,
				const std::string& fragment_path,
				const std::string& geometry_path = "");

		private:

			Shader() noexcept;

			std::uint32_t m_ID{};

			Shader_Type
				m_vertex{},
				m_fragment{},
				m_geometry{};

		}; // Shader

	} // gfx

} // tilia

#endif // TILIA_SHADER_HPP