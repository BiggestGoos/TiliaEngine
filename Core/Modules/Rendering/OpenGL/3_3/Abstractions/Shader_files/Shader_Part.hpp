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
#include "Core/Values/OpenGL/3_3/Enums.hpp"

namespace tilia {

	namespace gfx {

		// Predefinition
		class Shader;

		class Shader_Part {
		public:

			Shader_Part() = default;

			~Shader_Part();

			friend class Shader;

			Shader_Part(const std::string& path, const enums::Shader_Type& type, const bool& init = false) : m_type{ type }, m_path{ path } { if (init) Init(true); }
			Shader_Part(std::string&& path, const enums::Shader_Type& type, const bool& init = false) : m_type{ type }, m_path{ std::move(path) } { if (init) Init(true); }

			Shader_Part(const Shader_Part& other, const bool& init = false) noexcept :
				m_type{ other.m_type },
				m_path{ other.m_path },
				m_source{ other.m_source } { if (init) Init(true); }

			Shader_Part(Shader_Part&& other, const bool& init = false) noexcept :
				m_ID{ other.m_ID },
				m_type{ other.m_type },
				m_path{ std::move(other.m_path) },
				m_source{ std::move(other.m_source) } { other.m_ID = 0; if (init) Init(true); }

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

			void Init(const bool& reload = false);

			void Source();

			void Compile(const bool& source = false);

		private:

			// The ID of the Shader_Part
			std::uint32_t m_ID{};

			// The type of the Shader_Part
			enums::Shader_Type m_type{};

			// Path and source code of the Shader_Part
			std::string m_path{}, m_source{};

			std::vector<Shader*> m_attached_to{};

		};

	} // gfx

} // tilia

#endif // TILIA_SHADER_PART_HPP