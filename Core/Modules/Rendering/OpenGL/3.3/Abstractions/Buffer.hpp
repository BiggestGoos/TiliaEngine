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
		 * @brief An abstraction for a general openGL buffer.
		 */
		class Buffer 
		{
		private:

			// Alias for byte assuming char is 1 byte.
			using Byte = char;

		public:

			/**
			 * @brief Initializes the buffer with the given values. If a size larger than zero is given then we also allocate memory for the buffer.
			 *
			 * @param type - The buffer type which this will be initialized as.
			 * @param size - The size which the buffer will be allocated for in bytes.
			 * @param access_type - The access type that the memory will be accessed with.
			 * @param access_frequency - The frequency that the memory will be accessed.
			 * @param data - The data which will be copied into the buffer at initialization. Can be nullptr (is at default).
			 * @param allocate_local - Whether or not we will allocate a local buffer which we can write to instead of the openGL buffer and which we then can upload to openGL at once.
			 */
			void Init(const enums::Buffer_Type& type, const std::size_t& size = 0, const enums::Buffer_Access_Type& access_type = enums::Buffer_Access_Type::Draw, const enums::Buffer_Access_Frequency& access_frequency = enums::Buffer_Access_Frequency::Dynamic, const void* data = nullptr, const bool& allocate_local = false);

			/**
			 * @brief Deletes both the openGL buffer and if there is any the local buffer.
			 */
			void Terminate();

			/**
			 * @brief Allocates an openGL buffer which can then be accessed by either writing to or reading from it. If decided upon then also allocates a local buffer.
			 *
			 * @param size - The size which the buffer will be allocated for in bytes.
			 * @param access_type - The access type that the memory will be accessed with.
			 * @param access_frequency - The frequency that the memory will be accessed.
			 * @param data - The data which will be copied into the buffer at initialization. Can be nullptr.
			 * @param allocate_local - Whether or not we will allocate a local buffer which we can write to instead of the openGL buffer and which we then can upload to openGL at once.
			 */
			void Allocate(const std::size_t& size, const enums::Buffer_Access_Type& access_type, const enums::Buffer_Access_Frequency& access_frequency, const void* data = nullptr, const bool& allocate_local = false);

			/**
			 * @brief Allocates a local buffer which we can write to instead of the openGL side buffer. We can then upload the local buffer to said openGL buffer at once. If we set data in the local buffer then we need to also upload the data to openGL in order for it to take effect.
			 *
			 * @param size - The size of the buffer to be allocated.
			 * @param data - The data which will be copied into the buffer at initialization. Can be nullptr.
			 */
			void Allocate_Local(const std::size_t& size, const void* data = nullptr);

			/**
			 * @brief Set the data of either the opengl or the local buffer at the given offset and with the size.
			 *
			 * @param offset - The offset into the buffer that we set data for.
			 * @param size - The size of the data we want to set in bytes.
			 * @param data - The data to copy into the buffer.
			 * @param set_local - Whether or not we want to set the openGL or the local buffer. If no local buffer already exists then one is created.
			 */
			void Set_Data(const std::size_t& offset, const std::size_t& size, const void* data, const bool& set_local = false);

			/**
			 * @brief Gets the data from either the openGL or the local buffer at the given offset and with the size. 
			 *
			 * @param offset - The offset into the buffer that we get data from.
			 * @param size - The size of the data we want to get in bytes.
			 * @param data - The pointer which to copy the data from the buffer to.
			 * @param set_local - Whether or not we want to get from the openGL or the local buffer. If no local buffer exists then data is set to all zero.
			 */
			void Get_Data(const std::size_t& offset, const std::size_t& size, void* data, const bool& get_local = false);

			/**
			 * @brief Sets a given pointer to point towards either the openGL or the local buffer. If we want to point to the openGL buffer then we also need to specify a mapping type. This mapping type decides what you will be able to do with the mapped buffer. If we want to map to the local buffer we don't need to specify a mapping type. If we map to the openGL buffer we will also have to unmap the buffer. We don't have to unmap the buffer if we map to the local buffer but we do have to upload the data to openGL for the changes to take effect.
			 *
			 * @param map_type - The way we want to map to the openGL buffer.
			 * @param data - The return parameter which will be set to a pointer to the desired buffer.
			 * @param map_local - Whether or not we want to map to the openGL or the local buffer. If no local buffer already exists then one is created.
			 */
			void Map_Data(const enums::Buffer_Map_Type& map_type, void*& data, const bool& map_local = false);

			/**
			 * @brief Unmaps the mapped pointer which invalidates it and uploads the data.
			 */
			void Unmap_Data();

			/**
			 * @brief Uploads the data stored in the local buffer to openGL.
			 */
			void Upload_Data() const;

			/**
			 * @brief Binds the buffer.
			 */
			inline void Bind() const {
				// We call the static version with our member type and id in order to bind this buffer
				Bind(m_type, m_ID);
			}

			/**
			 * @brief Binds the buffer.
			 *
			 * @param type - The type of buffer to bind.
			 * @param id - The id of the buffer to bind.
			 */
			static void Bind(const enums::Buffer_Type& type, const std::uint32_t& id);

			/**
			 * @brief Unbinds the buffer.
			 *
			 * @param save_id - Whether or not to save the previous buffers id which will then be able to be rebound.
			 */
			inline void Unbind(const bool& save_id = false) const {
				// We call the static version with our type and given save id in order to unbind this buffer
				Unbind(m_type, save_id);
			}

			/**
			 * @brief Unbinds the buffer.
			 *
			 * @param type - The type of buffer to unbind.
			 * @param save_id - Whether or not to save the previous buffers id which will then be able to be rebound.
			 */
			static void Unbind(const enums::Buffer_Type& type, const bool& save_id = false);

			/**
			 * @brief Rebinds the stored buffer.
			 */
			inline void Rebind() const {
				// We call the static version with our type in order to rebind the old buffer
				Rebind(m_type);
			}

			/**
			 * @brief Rebinds the stored buffer.
			 *
			 * @param type - The type of the buffer to rebind.
			 */
			static void Rebind(const enums::Buffer_Type& type);

			/**
			 * @brief Gets the openGL id of the buffer.
			 */
			inline auto Get_ID() const { return m_ID; }

			/**
			 * @brief Gets the size of the buffer in bytes.
			 */
			inline auto Get_Size() const { return m_memory_size; }

			/**
			 * @brief Gets the type of the buffer.
			 */
			inline auto Get_Type() const { return m_type; }

			/**
			 * @brief Gets the acceess type of the buffer.
			 */
			inline auto Get_Access_Type() const { return m_access_type; }

			/**
			 * @brief Gets the acceess frequency of the buffer.
			 */
			inline auto Get_Access_Frequency() const { return m_access_frequency; }

			/**
			 * @brief Gets the mapping type of the buffer.
			 */
			inline auto Get_Map_Type() const { return m_map_type; }

		protected:

			// The id of the underlying openGL object.
			std::uint32_t m_ID{};
			// The local buffer which can be written to, read from, and mapped to instead of the openGL buffer.
			std::unique_ptr<Byte[]> m_local_data{};
			// The size of the buffer in bytes.
			std::size_t m_memory_size{};
			// The type of openGL buffer.
			enums::Buffer_Type m_type{};
			// The way we wish to access the allocated openGL memory block.
			enums::Buffer_Access_Type m_access_type{};
			// The frequency we wish to access the allocated openGL memory block.
			enums::Buffer_Access_Frequency m_access_frequency{};
			// The way that we have mapped to the allocated openGL memory block.
			enums::Buffer_Map_Type m_map_type{};

			// The currently bound buffer of each type.
			static std::map<enums::Buffer_Type, std::uint32_t> s_bound_IDs;
			// The saved buffer of each type.
			static std::map<enums::Buffer_Type, std::uint32_t> s_saved_IDs;

		}; // Buffer

	} // gfx

} // tilia

#endif // TILIA_BUFFER_HPP