/**************************************************************************************************
 * @file   Buffer.hpp
 * 
 * @brief  The Buffer class is an abstraction of a general openGL buffer.
 * 
 * @author Gustav Fagerlind
 * @date   20/12/2022
 *************************************************************************************************/

#ifndef TILIA_OPENGL_3_3_BUFFER_HPP
#define TILIA_OPENGL_3_3_BUFFER_HPP

// Standard
#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include <memory>
#include <optional>
#include <functional>

// Tilia
#include "Core/Values/Directories.hpp"
#define TILIA_INCLUDE_OPENGL_3_3_CONSTANTS
#include TILIA_CONSTANTS_INCLUDE

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

			// Alias for byte.
			using Byte = std::uint8_t;

		public:

			Buffer() = default;

			/**
			 * @brief Copy constructor. Initializes with resources from the given buffer.
			 * 
			 * @param other - The buffer to copy from.
			 */
			Buffer(const Buffer& other);

			/**
			 * @brief Move constructor. Moves resources from the given buffer. Leaves given
			 * buffer unusable.
			 * 
			 * @param other - The buffer to move from. Will be left unusable.
			 */
			Buffer(Buffer&& other) noexcept;

			/**
			 * @brief Copy assignment operator. Terminates this buffer before then initializing
			 * with resources from the given buffer.
			 * 
			 * @param other - The buffer to copy from.
			 * 
			 * @return This object.
			 */
			Buffer& operator=(const Buffer& other);

			/**
			 * @brief Move assignment operator. Terminates this buffer before then moving resources
			 * from the given buffer. Leaves given buffer unusable.
			 * 
			 * @param other - The buffer to move from. Will be left unusable.
			 * 
			 * @return This object.
			 */
			Buffer& operator=(Buffer&& other) noexcept;

			/**
			 * @brief Initializes the buffer with the given values. If a size larger than zero is
			 * given then we also allocate memory for the buffer.
			 *
			 * @param type - The buffer type which this will be initialized as.
			 * @param size - The size which the buffer will be allocated for in bytes.
			 * @param access_type - The access type that the memory will be accessed with.
			 * @param access_frequency - The frequency that the memory will be accessed.
			 * @param data - The data which will be copied into the buffer at initialization. Can
			 * be nullptr (is at default).
			 * @param allocate_local - Whether or not we will allocate a local buffer which we can
			 * write to instead of the openGL buffer and which we then can upload to openGL at
			 * once.
			 */
			void Init(const enums::Buffer_Type& type, const std::size_t& size = 0, 
				const enums::Buffer_Access_Type& access_type = enums::Buffer_Access_Type::Draw, 
				const enums::Buffer_Access_Frequency& access_frequency = 
				enums::Buffer_Access_Frequency::Dynamic, const void* data = nullptr, 
				const bool& allocate_local = false);

			/**
			 * @brief Deletes both the openGL and the local buffer.
			 */
			void Terminate();
			
			/**
			 * @brief Allocates an openGL buffer which can then be accessed by either writing to or
			 * reading from it. If decided upon then also allocates a local buffer.
			 *
			 * @param size - The size which the buffer will be allocated for in bytes.
			 * @param access_type - The access type that the memory will be accessed with.
			 * @param access_frequency - The frequency that the memory will be accessed.
			 * @param data - The data which will be copied into the buffer at initialization. Can
			 * be nullptr.
			 * @param allocate_local - Whether or not we will allocate a local buffer which we can 
			 * write to instead of the openGL buffer and which we then can upload to openGL at
			 * once.
			 */
			void Allocate(const std::size_t& size, const enums::Buffer_Access_Type& access_type, 
				const enums::Buffer_Access_Frequency& access_frequency, const void* data = nullptr, 
				const bool& allocate_local = false);

			/**
			 * @brief Deallocates all buffers.
			 */
			void Deallocate();

			/**
			 * @brief Allocates a local buffer which we can write to instead of the openGL side
			 * buffer. We can then upload the local buffer to said openGL buffer at once. If we set
			 * data in the local buffer then we need to also upload the data to openGL in order for
			 * it to take effect.
			 *
			 * @param size - The size of the buffer to be allocated.
			 * @param data - The data which will be copied into the buffer at initialization. Can
			 * be nullptr.
			 */
			void Allocate_Local(const std::size_t& size, const void* data = nullptr);

			/**
			 * @brief Set the data of either the opengl or the local buffer at the given offset and
			 * with the size.
			 *
			 * @param offset - The offset into the buffer that we set data for.
			 * @param size - The size of the data we want to set in bytes.
			 * @param data - The data to copy into the buffer.
			 * @param set_local - Whether or not we want to set the openGL or the local buffer. If
			 * no local buffer already exists then one is created.
			 */
			void Set_Data(const std::size_t& offset, const std::size_t& size, const void* data, 
				const bool& set_local = false);

			/**
			 * @brief Set the data of either the opengl or the local buffer.
			 *
			 * @param data - The data to copy into the buffer.
			 * @param set_local - Whether or not we want to set the openGL or the local buffer. If
			 * no local buffer already exists then one is created.
			 */
			inline void Set_Data(const void* data, const bool& set_local = false)
			{
				Set_Data(0, m_memory_size, data, set_local);
			}

			/**
			 * @brief Gets the data from either the openGL or the local buffer at the given offset
			 * and with the size. 
			 *
			 * @param offset - The offset into the buffer that we get data from.
			 * @param size - The size of the data we want to get in bytes.
			 * @param data - The pointer which to copy the data from the buffer to.
			 * @param set_local - Whether or not we want to get from the openGL or the local 
			 * buffer. If no local buffer exists then data is set to all zero.
			 */
			void Get_Data(const std::size_t& offset, const std::size_t& size, void* data, 
				const bool& get_local = false);

			/**
			 * @brief Gets the data from either the openGL or the local buffer at the given offset
			 * and with the size.
			 *
			 * @param data - The pointer which to copy the data from the buffer to.
			 * @param set_local - Whether or not we want to get from the openGL or the local
			 * buffer. If no local buffer exists then data is set to all zero.
			 */
			inline void Get_Data(void* data, const bool& get_local = false)
			{
				Get_Data(0, m_memory_size, data, get_local);
			}

			// Return value from Map_Data which will call Unmap_Data upon destruction or if 
			// re-assigned/reset is called.
			using Map_Value = std::unique_ptr<Byte, std::function<void(Byte*)>>;

			/**
			 * @brief Sets a given pointer to point towards either the openGL or the local buffer. 
			 * The openGL buffer can be mapped to with different mapping types. This decides in
			 * what ways the openGL buffer mapped data can be used.
			 * 
			 * The buffer has to be unmapped for this function to work. Therefore you should call
			 * Unmap_Data after you're fisihed with the mapped data.
			 * 
			 * @param mapping_type - The way we want to map to the openGL buffer or if we want to
			 * map to local.
			 * @param data - The return parameter which will be set to a pointer to the chosen 
			 * buffer.
			 */
			void Map_Data(const enums::Buffer_Map_Type& mapping_type, void*& data);

			/**
			 * @brief 
			 * 
			 * @param mapping_type
			 * 
			 * @return 
			 */
			std::optional<Map_Value> Map_Data_Auto(const enums::Buffer_Map_Type& mapping_type);

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
			 * 
			 * @param save_id - Whether or not to save the previously bound id for possible
			 * rebinding.
			 */
			inline void Bind(const bool& save_id = false) const {
				Bind(m_type, m_ID, save_id);
			}

			/**
			 * @brief Binds the buffer.
			 *
			 * @param type - The type of buffer to bind.
			 * @param id - The id of the buffer to bind.
			 * @param save_id - Whether or not to save the previously bound id for possible 
			 * rebinding.
			 */
			static void Bind(const enums::Buffer_Type& type, const std::uint32_t& id, 
				const bool& save_id = false);

			/**
			 * @brief Unbinds the buffer.
			 *
			 * @param save_id - Whether or not to save the previous buffers id which will then be
			 * able to be rebound.
			 */
			inline void Unbind(const bool& save_id = false) const {
				Unbind(m_type, save_id);
			}

			/**
			 * @brief Unbinds the buffer.
			 *
			 * @param type - The type of buffer to unbind.
			 * @param save_id - Whether or not to save the previous buffers id which will then be
			 * able to be rebound.
			 */
			static void Unbind(const enums::Buffer_Type& type, const bool& save_id = false);

			/**
			 * @brief Rebinds the stored buffer.
			 */
			inline void Rebind() const {
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

#if TILIA_UNIT_TESTS == 1

			/**
			 * @brief Unit test for openGL 3.3 Buffer. Has to be called after creation of an openGL
			 * context.
			 */
			static void Test();

#endif // TILIA_UNIT_TESTS == 1

		private:

			// The id of the underlying openGL object.
			std::uint32_t m_ID{};

			// The local buffer which can be written to, read from, and mapped to instead of the
			// openGL buffer.
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
			static std::unordered_map<std::uint32_t, std::uint32_t> s_bound_IDs;
			// The saved buffer of each type.
			static std::unordered_map<std::uint32_t, std::uint32_t> s_saved_IDs;

		}; // Buffer

	} // gfx

} // tilia

#endif // TILIA_OPENGL_3_3_BUFFER_HPP