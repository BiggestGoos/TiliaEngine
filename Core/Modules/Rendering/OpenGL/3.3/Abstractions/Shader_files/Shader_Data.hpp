/**************************************************************************************************
 * @file   Shader_Data.h
 * @brief  Holds a class which holds data to be set as uniforms in an 
 *		   @Shader object.

 * @author Gustav Fagerlind
 * @date   05/07/2022
 *************************************************************************************************/
#if 0
#ifndef TILIA_SHADER_DATA_HPP
#define TILIA_SHADER_DATA_HPP

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

		/**
		 * @brief Small container class for Shader_Data class. Contains the path to and the source code of an openGL shader.
		 */
		struct Shader_Part {

			// The ID of the Shader_Part
			std::uint32_t ID{};

			// Path and source code of shader
			std::string path{}, source{};

			Shader_Part(const std::string& path) : path{ path } { }
			Shader_Part(std::string&& path) : path{ std::move(path) } { }

			Shader_Part(const Shader_Part& other) noexcept :
				path{ other.path }, 
				source{ other.source } { }

			Shader_Part(Shader_Part&& other) noexcept : 
				ID{ other.ID },
				path{ std::move(other.path) }, 
				source{ std::move(other.source) } { other.ID = 0; }

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

				this->ID = other.ID;
				other.ID = 0;
				this->path = std::move(other.path);
				this->source = std::move(other.source);

				return *this;
			}

		};

		/**
		 * @brief Abstraction for openGL shader. Backend for Shader class.
		 */
		class Shader_Data {
		public:

			/**
			 * @brief Copy-contructor which calls default construcotr and copies from given Shader_Data.
			 * 
			 * @param other - The given Shader_Data
			 */
			Shader_Data(const Shader_Data& other) noexcept
			{ 
				Generate_Shader();
				m_parts = other.m_parts; 
				m_location_cache = other.m_location_cache;
				m_use_geometry = other.m_use_geometry;
			}
			/**
			 * @brief Move-constructor which moves all resources from given Shader_Data and then leaves given Shader_Data useless.
			 * 
			 * @param other 
			 */
			Shader_Data(Shader_Data&& other) noexcept
			{
				m_parts = std::move(other.m_parts);
				m_ID = other.m_ID;
				other.m_ID = 0;
				m_location_cache = std::move(other.m_location_cache);
				m_use_geometry = other.m_use_geometry;
			}

			~Shader_Data() noexcept;

			Shader_Data& operator=(const Shader_Data& other) noexcept
			{
				if (&other == this)
					return *this;

				

				Generate_Shader();
				m_parts = other.m_parts;
				m_location_cache = other.m_location_cache;
				m_use_geometry = other.m_use_geometry;
				
				return *this;
			}

			Shader_Data& operator=(Shader_Data&& other) noexcept
			{
				if (&other == this)
					return *this;

				m_parts = std::move(other.m_parts);
				m_ID = other.m_ID;
				other.m_ID = 0;
				m_location_cache = std::move(other.m_location_cache);
				m_use_geometry = other.m_use_geometry;

				return *this;
			}

			inline auto Get_ID() {
				return m_ID;
			}

			//inline void Set_Part(const Shader_Part& part, const enums::Shader_Type& type, const bool& reload = false) {
			//	m_parts[utils::Get_Shader_Type_Index(type)] = part;
			//	if (reload)
			//		Reload(type);
			//}

			//inline void Set_Part(Shader_Part&& part, const enums::Shader_Type& type, const bool& reload = false) {
			//	m_parts[utils::Get_Shader_Type_Index(type)] = std::move(part);
			//	if (reload)
			//		Reload(type);
			//}

			inline auto Get_Part(const enums::Shader_Type& type) {
				return m_parts[utils::Get_Shader_Type_Index(type)];
			}

			inline void Reload(const enums::Shader_Type& type) {
				Reload(utils::Get_Shader_Type_Index(type));
			}

			/**
			 * @brief Binds the shader.
			 */
			void Bind() const;

			/**
			 * @brief Binds the given shader.
			 * 
			 * @param id - The id of the shader which will be bound.
			 */
			static void Bind(const std::uint32_t& id);

			/**
			 * @brief Unbinds the bound shader. Static version
			 * of Unbind.
			 */
			static void Unbind(const bool& save_id = false);

			/**
			 * @brief Binds the stored previously bound shader. Static version
			 * of Rebind.
			 */
			static void Rebind();

			void Uniform(const std::string& loc, std::initializer_list<float> vs);
			void Uniform(const std::string& loc, std::initializer_list<std::int32_t> vs);
			void Uniform(const std::string& loc, std::initializer_list<std::uint32_t> vs);

			template<glm::length_t size>
			void Uniform(const std::string& loc, glm::vec<size, float, glm::defaultp> v) {
				Uniform(loc, &v[0], size);
			}

			template<glm::length_t size>
			void Uniform(const std::string& loc, glm::vec<size, std::int32_t, glm::defaultp> v) {
				Uniform(loc, &v[0], size);
			}

			template<glm::length_t size>
			void Uniform(const std::string& loc, glm::vec<size, std::uint32_t, glm::defaultp> v)
			{
				Uniform(loc, &v[0], size);
			}

			void Uniform(const std::string& loc, const float* vs, const std::size_t& size);

			void Uniform(const std::string& loc, const std::int32_t* vs, const std::size_t& size);

			void Uniform(const std::string& loc, const std::uint32_t* vs, const std::size_t& size);

			template<glm::length_t size>
			void Uniform(const std::string& loc, glm::mat<size, size, float, glm::defaultp> v) {
				Uniform(loc, &v[0], size * size);
			}

			template<glm::length_t size>
			void Uniform(const std::string& loc, glm::mat<size, size, std::int32_t, glm::defaultp> v) {
				Uniform(loc, &v[0], size * size);
			}

			template<glm::length_t size>
			void Uniform(const std::string& loc, glm::mat<size, size, std::uint32_t, glm::defaultp> v) {
				Uniform(loc, &v[0], size * size);
			}

		protected:

			Shader_Data(const std::initializer_list<Shader_Part>& vertex_parts, const std::initializer_list<Shader_Part>& fragment_parts, const std::initializer_list<Shader_Part>& geometry_parts, const bool& use_geometry) noexcept {
				Generate_Shader();
				m_parts = { vertex_parts, fragment_parts, geometry_parts };
				m_use_geometry = use_geometry;
			}

			void Generate_Shader() noexcept;
			void Delete_Shader() noexcept;

			void Reload(const std::size_t& index = 3);

			std::uint32_t m_ID{};
			
			std::array<std::vector<Shader_Part>, 3> m_parts{};
			
			std::unordered_map<std::string, std::int32_t> m_location_cache{};

		private:

			std::int32_t Get_Uniform_Location(const std::string& name);

			std::uint32_t Make_Shader(const tilia::enums::Shader_Type& type);

			bool m_use_geometry{};

			static std::uint32_t s_bound_ID; 

			static std::uint32_t s_previous_ID; 

		}; // Shader_Data

} // gfx

} // tilia

#endif // TILIA_SHADER_DATA_HPP
#endif