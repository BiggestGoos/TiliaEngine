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

		/**
		 * @brief Abstraction for openGL shader. Interface for the backend class Shader_Data which this class derives from.
		 */
		template<bool use_geometry>
		class Shader : public Shader_Data {
		public:

			Shader() 
				: m_shader_data{ { }, use_geometry }
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
			Shader(const Shader_Part& vertex, const Shader_Part& fragment, const bool& reload = false)
				: m_shader_data{ { vertex, fragment }, use_geometry }
			{
				if (reload)
					m_shader_data.Reload();
			}

			/**
			 * @brief Version of constructor in which the shader does use geometry shader.
			 * 
			 * @param vertex - The vertex shader part.
			 * @param fragment - The fragment shader part.
			 * @param geometry - The geometry shader part.
			 * @param reload - Wheter or not to reload the Shader at the end of construction.
			 * 
			 * @editnote Uses std::enable_if_t to deduce which constructor to call depending on if the object uses geometry shader.
			 */
			template <bool B = use_geometry,
				std::enable_if_t<(B == true)>* = nullptr>
			Shader(const Shader_Part& vertex, const Shader_Part& fragment, const Shader_Part& geometry, const bool& reload = false)
				: m_shader_data{ { vertex, fragment, geometry }, use_geometry }
			{
				if (reload)
					m_shader_data.Reload();
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
				m_shader_data.m_parts = { vertex, fragment };
				if (reload)
					m_shader_data.Reload();
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
				m_shader_data.m_parts = { vertex, fragment, geometry };
				if (reload)
					m_shader_data.Reload();
			}

			Shader(const Shader& other){
				m_shader_data = other.m_shader_data;
			}

			Shader(Shader&& other){
				m_shader_data = std::move(other.m_shader_data);
			}

		private:

			Shader_Data m_shader_data{};

		}; // Shader

	} // gfx

} // tilia

#endif // TILIA_SHADER_HPP