/**************************************************************************************************
 * @file   Shader.hpp
 * 
 * @brief  Declares an interface class for an abstraction of an openGL shader. Derives from the 
 *		   Shader_Data class which contains the backend of the shader abstraction.
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
				: Shader_Data{ { }, use_geometry }
			{ }

			template <bool B = use_geometry,
				std::enable_if_t<(B == false)>* = nullptr>
			Shader(const Shader_Part& vertex, const Shader_Part& fragment, const bool& reload = false)
				: Shader_Data{ { vertex, fragment }, use_geometry }
			{
				if (reload)
					Reload();
			}

			template <bool B = use_geometry,
				std::enable_if_t<(B == true)>* = nullptr>
			Shader(const Shader_Part& vertex, const Shader_Part& fragment, const Shader_Part& geometry, const bool& reload = false)
				: Shader_Data{ { vertex, fragment, geometry }, use_geometry }
			{
				if (reload)
					Reload();
			}

			template <bool B = use_geometry,
				std::enable_if_t<(B == false)>* = nullptr>
			void Reset(const Shader_Part& vertex, const Shader_Part& fragment, const bool& reload = false)
			{
				m_parts = { vertex, fragment };
				if (reload)
					Reload();
			}

			template <bool B = use_geometry,
				std::enable_if_t<(B == true)>* = nullptr>
				void Reset(const Shader_Part& vertex, const Shader_Part& fragment, const Shader_Part& geometry, const bool& reload = false)
			{
				m_parts = { vertex, fragment, geometry };
				if (reload)
					Reload();
			}

		}; // Shader

	} // gfx

} // tilia

#endif // TILIA_SHADER_HPP