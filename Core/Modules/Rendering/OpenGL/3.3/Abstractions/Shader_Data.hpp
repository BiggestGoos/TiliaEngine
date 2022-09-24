/*****************************************************************//**
 * @file   Shader_Data.h
 * @brief  Holds a class which holds data to be set as uniforms in an 
 *		   @Shader object.
 * 
 * @define TILIA_SHADER_DATA_H
 * 
 * Dependencies:
 * @include "dependencies/glm/include/glm/glm.hpp"
 * 
 * Standard:
 * @include <unordered_map>
 * @include <string>
 * 
 * @author Gustav Fagerlind
 * @date   05/07/2022
 *********************************************************************/

#ifndef TILIA_SHADER_DATA_H
#define TILIA_SHADER_DATA_H

 // Vendor
#include "vendor/glm/include/glm/glm.hpp"

// Standard
#include <unordered_map>
#include <string>
#include <array>
#include <vector>
#include <initializer_list>

namespace tilia {

	namespace gfx {

#if 0

		// The different types of uniforms
		enum class Uniform_Type {
			Float,
			Int,
			Uint,
			Vec,
			Ivec,
			Uvec,
			Float_Arr,
			Int_Arr,
			Uint_Arr,
			Mat
		};

		/**
		 * @brief A class which holds uniform information which will be set when rendering.
		 */
		class Shader_Data {
		public:

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

			/**
			 * @brief Removes the uniform with this location.
			 * 
			 * @param location - The location of the uniform to remove.
			 */
			void Remove_Uniform(const std::string& location);

			/**
			 * @brief Deletes all allocated memory
			 */
			~Shader_Data();

		private:

			/**
			 * @brief Casts the void pointer back to type and then deletes.
			 * 
			 * @param location - The location in the unordered_map to delete.
			 */
			void Delete_Uniform(const std::string& location);

			/**
			 * @brief Holds what type, the amount of and the data of a uniform.
			 */
			struct Uniform_Variable
			{
				Uniform_Type type{};
				size_t amount{};
				void* data{};
				/**
				 * @brief Compares two Uniform_Variable. If they are equal returns true.
				 */
				bool operator==(const Uniform_Variable& other) const;
				/**
				 * @brief Compares two Uniform_Variable. If they are equal returns false.
				 */
				bool operator!=(const Uniform_Variable& other) const {
					return !(*this == other);
				}
			};

		public:

			// Holds the set uniform variables
			std::unordered_map<std::string, Uniform_Variable> uniform_variables{};

			/**
			 * @brief Compares two Shader_Data. If they are equal returns true.
			 */
			bool operator==(const Shader_Data& other) const {
				return (this->uniform_variables == other.uniform_variables);
			}
			/**
			 * @brief Compares two Shader_Data. If they are equal returns false.
			 */
			bool operator!=(const Shader_Data& other) const {
				return !(*this == other);
			}

		};

#endif // 0

		class Shader_Data {
		public:

			std::string path{};
			std::string source{};

			Shader_Data() = default;

			Shader_Data(const std::string& path)
				: path{ path }
			{
			}

			Shader_Data(const std::string& path, const std::string& source)
				: path{ path },
				source{ source }
			{
			}

			Shader_Data(const Shader_Data& other) noexcept
				: path{ other.path },
				source{ other.source }
			{
			}

			Shader_Data(Shader_Data&& other) noexcept
			: path{ std::move(other.path) },
				source{ std::move(other.source) }
			{
			}

			Shader_Data& operator=(const Shader_Data& other) noexcept
			{
				if (&other == this)
					return *this;

				this->path = other.path;
				this->source = other.source;
				
				return *this;
			}

			Shader_Data& operator=(Shader_Data&& other) noexcept
			{
				if (&other == this)
					return *this;

				this->path = std::move(other.path);
				this->source = std::move(other.source);

				return *this;
			}

		};

	} // gfx

} // tilia

#endif // TILIA_SHADER_DATA_H