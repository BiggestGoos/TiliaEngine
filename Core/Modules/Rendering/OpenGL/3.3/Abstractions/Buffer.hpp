/**************************************************************************************************
 * @file   Buffer.hpp
 * 
 * @brief  The Buffer class is an abstraction of a general openGL buffer.
 * 
 * @author Gustav Fagerlind
 * @date   20/12/2022
 *************************************************************************************************/

#ifndef TILIA_BUFFER_HPP
#define TILIA_BUFFER_HPP

// Vendor

// Standard
#include <cstdint>
#include <cstddef>
#include <map>
#include <memory>

// Tilia
#include "Core/Values/OpenGL/3.3/Enums.hpp"

namespace tilia
{
	namespace gfx 
	{
		/**
		 * // TODO: finnish writing comments etc, look up how to structure unit tests and implement one for buffer
		 */
		class Buffer 
		{
		private:

			// Alias for byte assuming char is 1 byte.
			using Byte = char;

		public:

			void Init(const enums::Buffer_Type& type, const std::size_t& size, const enums::Buffer_Access_Type& access_type = enums::Buffer_Access_Type::Draw, const enums::Buffer_Access_Frequency& access_frequency = enums::Buffer_Access_Frequency::Dynamic, const void* data = nullptr, const bool& allocate_local = false);

			void Terminate();

			void Allocate(const std::size_t& size, const enums::Buffer_Access_Type& access_type, const enums::Buffer_Access_Frequency& access_frequency, const void* data = nullptr, const bool& allocate_local = false);

			void Allocate_Local(const std::size_t& size, const void* data = nullptr);

			void Set_Data(const std::size_t& offset, const std::size_t& size, const void* data, const bool& set_local = false);

			void Get_Data(const std::size_t& offset, const std::size_t& size, void* data, const bool& get_local = false);

			void Map_Data(const enums::Buffer_Map_Type& map_type, void*& data, const bool& map_local = false);

			void Unmap_Data();

			void Upload_Data() const;

			inline void Bind() const {
				// We call the static version with our member type and id in order to bind this buffer
				Bind(m_type, m_ID);
			}

			static void Bind(const enums::Buffer_Type& type, const std::uint32_t& id);

			inline void Unbind(const bool& save_id = false) const {
				// We call the static version with our type and given save id in order to unbind this buffer
				Unbind(m_type, save_id);
			}

			static void Unbind(const enums::Buffer_Type& type, const bool& save_id = false);

			inline void Rebind() const {
				// We call the static version with our type in order to rebind the old buffer
				Rebind(m_type);
			}

			static void Rebind(const enums::Buffer_Type& type);

			inline auto Get_ID() const { return m_ID; }

			inline auto Get_Size() const { return m_memory_size; }

			inline void Set_Auto_Upload(const bool& auto_upload) { m_auto_upload = auto_upload; }

			inline auto Get_Auto_Upload() const { return m_auto_upload; }

			inline auto Get_Type() const { return m_type; }

			inline auto Get_Access_Type() const { return m_access_type; }

			inline auto Get_Access_Frequency() const { return m_access_frequency; }

			inline auto Get_Map_Type() const { return m_map_type; }

		protected:

			std::uint32_t m_ID{};

			std::unique_ptr<Byte[]> m_local_data{};

			std::size_t m_memory_size{};

			bool m_auto_upload{};

			enums::Buffer_Type m_type{};

			enums::Buffer_Access_Type m_access_type{};

			enums::Buffer_Access_Frequency m_access_frequency{};

			enums::Buffer_Map_Type m_map_type{};

			// The currently bound buffer of each type.
			static std::map<enums::Buffer_Type, std::uint32_t> s_bound_IDs;
			// The saved buffer of each type.
			static std::map<enums::Buffer_Type, std::uint32_t> s_saved_IDs;

		}; // Buffer

	} // gfx

} // tilia

#endif // TILIA_BUFFER_HPP