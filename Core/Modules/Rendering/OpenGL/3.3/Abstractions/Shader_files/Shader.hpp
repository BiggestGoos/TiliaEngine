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
#include "vendor/glm/include/glm/gtc/type_ptr.hpp"

// Standard
#include <array>
#include <vector>
#include <unordered_map>
#include <string>
#include <type_traits>
#include <initializer_list>

// Tilia
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Shader_files/Shader_Part.hpp"
#include "Core/Values/OpenGL/3.3/Utils.hpp"

namespace tilia 
{

	namespace gfx 
	{

		/**
		 * @brief Abstraction for openGL shader. Backend for Shader class.
		 */
		class Shader {
		public:

			Shader() = default;

			~Shader();

			/**
			 * @brief Copy-contructor which calls default construcotr and copies from given Shader_Data.
			 *
			 * @param other - The given Shader_Data
			 */
			Shader(const Shader& other) noexcept
			{
				m_location_cache = other.m_location_cache;
			}
			/**
			 * @brief Move-constructor which moves all resources from given Shader_Data and then leaves given Shader_Data useless.
			 *
			 * @param other
			 */
			Shader(Shader&& other) noexcept
			{
				m_ID = other.m_ID;
				other.m_ID = 0;
				m_location_cache = std::move(other.m_location_cache);
			}

			Shader& operator=(const Shader& other) noexcept
			{
				if (&other == this)
					return *this;

				m_location_cache = other.m_location_cache;

				return *this;
			}

			Shader& operator=(Shader&& other) noexcept
			{
				if (&other == this)
					return *this;

				m_ID = other.m_ID;
				other.m_ID = 0;
				m_location_cache = std::move(other.m_location_cache);

				return *this;
			}

			void Init(std::initializer_list<std::weak_ptr<Shader_Part>> vertex_parts, std::initializer_list<std::weak_ptr<Shader_Part>> fragment_parts, std::initializer_list<std::weak_ptr<Shader_Part>> geometry_parts);

			void Add_Part(std::weak_ptr<Shader_Part> shader_part, const bool& reload = true);

			void Remove_Part(std::weak_ptr<Shader_Part> shader_part, const bool& reload = true);

			void Reload();

			inline auto Get_ID() { return m_ID; }

			void Bind_Uniform_Block(const std::string& block_name, const std::uint32_t& block_index);

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

			template<typename T,
			std::enable_if_t<std::is_same<float, T>::value || std::is_same<std::int32_t, T>::value || std::is_same<std::uint32_t, T>::value>* = nullptr>
			void Uniform(const std::string& loc, std::initializer_list<T> vs)
			{
				Uniform(loc, vs.begin(), vs.size());
			}

			template<typename T, glm::length_t size, glm::qualifier Q,
			std::enable_if_t<std::is_same<float, T>::value || std::is_same<std::int32_t, T>::value || std::is_same<std::uint32_t, T>::value>* = nullptr>
			void Uniform(const std::string& loc, glm::vec<size, T, Q> v)
			{
				Uniform(loc, glm::value_ptr(v), size);
			}

			template<glm::length_t size_x, glm::length_t size_y, glm::qualifier Q>
			void Uniform(const std::string& loc, glm::mat<size_x, size_y, float, Q> v)
			{
				Uniform(loc, glm::value_ptr(v), size_x, size_y);
			}

			void Uniform(const std::string& loc, const float* vs, const std::size_t& size);

			void Uniform(const std::string& loc, const std::int32_t* vs, const std::size_t& size);

			void Uniform(const std::string& loc, const std::uint32_t* vs, const std::size_t& size);

			void Uniform(const std::string& loc, const float* vs, const std::size_t& size_x, const std::size_t& size_y);

		private:

			std::uint32_t m_ID{};

			std::unordered_map<std::string, std::int32_t> m_location_cache{};

			std::int32_t Get_Uniform_Location(const std::string& name);

			std::int32_t Get_Uniform_Block_Index(const std::string& name);

			static std::uint32_t s_bound_ID;
			static std::uint32_t s_previous_ID;

		}; // Shader

	} // gfx

} // tilia

#endif // TILIA_SHADER_HPP