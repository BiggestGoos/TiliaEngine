/**************************************************************************************************
 * @file   Shader_Part.hpp
 * 
 * @brief  
 * 
 * @author Gustav Fagerlind
 * @date   03/10/2022
 *************************************************************************************************/

#ifndef TILIA_SHADER_PART_HPP
#define TILIA_SHADER_PART_HPP

// Standard
#include <cstdint>
#include <string>
#include <memory>

// Tilia
#include "Core/Values/OpenGL/3.3/Enums.hpp"

namespace tilia {

	namespace gfx {

		class Shader_Part {
		public:

			//friend class Shader;

			Shader_Part(const std::string& path, const enums::Shader_Type& type) : m_type{ type }, m_path{ path } { }
			Shader_Part(std::string&& path, const enums::Shader_Type& type) : m_type{ type }, m_path{ std::move(path) } { }

			Shader_Part(const Shader_Part& other) noexcept :
				m_type{ other.m_type },
				m_path{ other.m_path },
				m_source{ other.m_source } { }

			Shader_Part(Shader_Part&& other) noexcept :
				m_ID{ other.m_ID },
				m_type{ other.m_type },
				m_path{ std::move(other.m_path) },
				m_source{ std::move(other.m_source) } { other.m_ID = 0; }

			Shader_Part& operator=(const Shader_Part& other) noexcept
			{
				if (&other == this)
					return *this;

				m_type = other.m_type;
				m_path = other.m_path;
				m_source = other.m_source;

				return *this;
			}
			Shader_Part& operator=(Shader_Part&& other) noexcept
			{
				if (&other == this)
					return *this;

				m_ID = other.m_ID;
				other.m_ID = 0;
				m_type = other.m_type;
				m_path = std::move(other.m_path);
				m_source = std::move(other.m_source);

				return *this;
			}

			inline std::uint32_t Get_ID() const { return m_ID; }

			inline void Set_Type(const enums::Shader_Type& type) { m_type = type; }

			inline enums::Shader_Type Get_Type() const { return m_type; }

			inline void Set_Path(const std::string& path) { m_path = path; }
			inline void Set_Path(std::string&& path) { m_path = std::move(path); }

			inline std::string Get_Path() const { return m_path; }

			inline void Set_Source(const std::string& source) { m_source = source; }
			inline void Set_Source(std::string&& source) { m_source = std::move(source); }

			inline std::string Get_Source() const { return m_source; }

			void Init();

			void Source();

			void Compile();

		private:


			// The ID of the Shader_Part
			std::uint32_t m_ID{};

			// The type of the Shader_Part
			enums::Shader_Type m_type{};

			// Path and source code of the Shader_Part
			std::string m_path{}, m_source{};

		};

	} // gfx

} // tilia

#endif // TILIA_SHADER_PART_HPP