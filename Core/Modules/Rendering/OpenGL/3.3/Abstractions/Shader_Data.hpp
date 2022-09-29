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

// Tilia
#include "Core/Values/OpenGL/3.3/Enums.hpp"
#include "Core/Values/OpenGL/3.3/Utils.hpp"

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

#if 1

		struct Shader_Part {

			std::string path{}, source{};

			Shader_Part(const std::string& path) : path{ path } { }
			Shader_Part(std::string&& path) : path{ std::move(path) } { }

			Shader_Part(const Shader_Part& other) noexcept :
				path{ other.path }, 
				source{ other.source } { }

			Shader_Part(Shader_Part&& other) noexcept : 
				path{ std::move(other.path) }, 
				source{ std::move(other.source) } { }

			Shader_Part& operator=(const Shader_Part& other) noexcept
			{
				if (&other == this)
					return *this;

				this->path = other.path;
				this->source = other.source;

				return *this;
			}
			Shader_Part& operator=(Shader_Part&& other) noexcept
			{
				if (&other == this)
					return *this;

				this->path = std::move(other.path);
				this->source = std::move(other.source);

				return *this;
			}

		};

		class Shader_Data {
		public:

			Shader_Data() noexcept;

			Shader_Data(const Shader_Data& other) noexcept
				: Shader_Data()
			{

				m_parts = other.m_parts;

			}

			Shader_Data(Shader_Data&& other) noexcept
			{

				m_parts = std::move(other.m_parts);
				this->m_ID = other.m_ID;

			}

			Shader_Data(std::initializer_list<Shader_Part> parts, const bool& use_geometry) noexcept : Shader_Data() {
				m_parts = parts;
				m_use_geometry = use_geometry;
			}

			Shader_Data& operator=(const Shader_Data& other) noexcept
			{
				if (&other == this)
					return *this;

				m_parts = other.m_parts;
				m_ID = other.m_ID;
				
				return *this;
			}

			Shader_Data& operator=(Shader_Data&& other) noexcept
			{
				if (&other == this)
					return *this;

				m_parts = std::move(other.m_parts);
				m_ID = other.m_ID;
				other.m_ID = 0;

				return *this;
			}

			inline auto Get_ID() {
				return m_ID;
			}

			inline void Set_Part(const Shader_Part& part, const enums::Shader_Type& type, const bool& reload = false) {
				m_parts[utils::Get_Shader_Type_Index(type)] = part;
				if (reload)
					Reload(type);
			}

			inline void Set_Part(Shader_Part&& part, const enums::Shader_Type& type, const bool& reload = false) {
				m_parts[utils::Get_Shader_Type_Index(type)] = std::move(part);
				if (reload)
					Reload(type);
			}

			inline auto Get_Part(const enums::Shader_Type& type) {
				return m_parts[utils::Get_Shader_Type_Index(type)];
			}

			inline void Reload(const enums::Shader_Type& type) {
				Reload(utils::Get_Shader_Type_Index(type));
			}

		protected:

			std::uint32_t Make_Shader(const tilia::enums::Shader_Type& type);

			void Reload(const std::size_t& index = 3);

			std::uint32_t m_ID{};
			
			std::vector<Shader_Part> m_parts{};
			
			std::unordered_map<std::string, int32_t> m_location_cache{};

		private:

			bool m_use_geometry{};

			static uint32_t s_bound_ID; 

			static uint32_t s_previous_ID; 

		};

	} // gfx

#endif

} // tilia

#endif // TILIA_SHADER_DATA_H