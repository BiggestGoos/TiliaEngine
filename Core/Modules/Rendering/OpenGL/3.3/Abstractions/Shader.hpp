/**************************************************************************************************
 * @file   Shader.hpp
 * 
 * @brief  Declares an interface class for an abstraction of an openGL shader. Derives from the 
 *		   Shader_Data class which contains the backend of the shader abstraction.
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
#include <type_traits>
#include <initializer_list>

// Tilia
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Shader_Data.hpp"

namespace tilia {

	namespace gfx {

#if 0

		/**
		 * @brief Abstraction for openGL shader. Interface for the backend class Shader_Data which this class derives from.
		 */
		template<bool use_geometry>
		class Shader : public Shader_Data {
		public:

			Shader() 
				: Shader_Data{ { }, use_geometry }
			{ }

			/**
			 * @brief Version of constructor in which the shader doesn't use geometry shader.
			 * 
			 * @param vertex - The vertex shader part.
			 * @param fragment - The fragment shader part.
			 * @param reload - Wheter or not to reload the Shader at the end of construction.
			 * 
			 * @editnote Uses std::enable_if_t to deduce which constructor to call depending on if the object uses geometry shader.
			 */
			template <bool B = use_geometry,
				std::enable_if_t<(B == false)>* = nullptr>
			Shader(const std::initializer_list<Shader_Part>& vertex_parts, const std::initializer_list<Shader_Part>& fragment_parts, const bool& reload = false)
				: Shader_Data{ vertex_parts, fragment_parts, { }, use_geometry }
			{
				if (reload)
					Reload();
			}

			/**
			 * @brief Version of constructor in which the shader does use geometry shader.
			 * 
			 * @param vertex - The vertex shader parts.
			 * @param fragment - The fragment shader parts.
			 * @param geometry - The geometry shader parts.
			 * @param reload - Wheter or not to reload the Shader at the end of construction.
			 * 
			 * @editnote Uses std::enable_if_t to deduce which constructor to call depending on if the object uses geometry shader.
			 */
			template <bool B = use_geometry,
				std::enable_if_t<(B == true)>* = nullptr>
			Shader(const std::initializer_list<Shader_Part>& vertex_parts, const std::initializer_list<Shader_Part>& fragment_parts, const std::initializer_list<Shader_Part>& geometry_parts, const bool& reload = false)
				: Shader_Data{ vertex_parts, fragment_parts, geometry_parts, use_geometry }
			{
				if (reload)
					Reload();
			}

			/**
			 * @brief Version of Reset function in which the shader doesn't use geometry shader.
			 * 
			 * @param vertex - The vertex shader part.
			 * @param fragment - The fragment shader part.
			 * @param reload - Wheter or not to reload the Shader at the end of the function.
			 * 
			 * @editnote Uses std::enable_if_t to deduce which function to call depending on if the object uses geometry shader.
			 */
			template <bool B = use_geometry,
				std::enable_if_t<(B == false)>* = nullptr>
			void Reset(const Shader_Part& vertex, const Shader_Part& fragment, const bool& reload = false)
			{
				m_parts = { vertex, fragment };
				if (reload)
					Reload();
			}

			/**
			 * @brief Version of Reset function in which the shader does use geometry shader.
			 * 
			 * @param vertex - The vertex shader part.
			 * @param fragment - The fragment shader part.
			 * @param geometry - The geometry shader part.
			 * @param reload - Wheter or not to reload the Shader at the end of the function.
			 * 
			 * @editnote Uses std::enable_if_t to deduce which function to call depending on if the object uses geometry shader.
			 */
			template <bool B = use_geometry,
				std::enable_if_t<(B == true)>* = nullptr>
				void Reset(const Shader_Part& vertex, const Shader_Part& fragment, const Shader_Part& geometry, const bool& reload = false)
			{
				m_parts = { vertex, fragment, geometry };
				if (reload)
					Reload();
			}

			//Shader(const Shader& other) {
			//	*this = other;
			//}

			//Shader(Shader&& other) {
			//	*this = other;
			//}

		}; // Shader

#endif // 0

#if 1

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
		class Shader {
		public:

			/**
			 * @brief Copy-contructor which calls default construcotr and copies from given Shader_Data.
			 *
			 * @param other - The given Shader_Data
			 */
			Shader(const Shader& other) noexcept
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
			Shader(Shader&& other) noexcept
			{
				m_parts = std::move(other.m_parts);
				m_ID = other.m_ID;
				other.m_ID = 0;
				m_location_cache = std::move(other.m_location_cache);
				m_use_geometry = other.m_use_geometry;
			}

			~Shader() noexcept;

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

		}; // Shader

#endif

	} // gfx

} // tilia

#endif // TILIA_SHADER_HPP