/**************************************************************************************************
 * @file   Image.hpp
 * 
 * @brief  Class for image loading and handling.
 * 
 * @author Gustav Fagerlind
 * @date   03/04/2023
 *************************************************************************************************/

#ifndef TILIA_IMAGE_HPP
#define TILIA_IMAGE_HPP

// Standard
#include <memory>
#include <string>

// Tilia
#include "Core/Values/Directories.hpp"
#include TILIA_CONSTANTS_INCLUDE

namespace tilia
{
	namespace enums
	{
		// The different types of image channels that an image can be loaded with.
		enum class Image_Channels
		{
			Largest    = 0x0000,
			Gray	   = 0x0001,
			Gray_Alpha = 0x0002,
			RGB		   = 0x0003,
			RGBA	   = 0x0004
		}; // Image_Channels
	} // enums

	/**
	 * @brief A class for image loading. Can be copied and moved for both construction and
	 * assignment.
	 */
	class Image
	{
	private:

		using Byte = uint8_t;

		void Copy_Data(const Image& other)
		{
			const auto size{ other.m_width * other.m_height * (*other.m_image_channels) };
			// Copies data as well as sets custom destructor to just do 'delete[]' since the data
			// is allocated using new[]
			m_image_data = { new Byte[size], [](Byte* image_data) { delete[] image_data; } };
			std::copy(other.m_image_data.get(),
				other.m_image_data.get() + size, m_image_data.get());
		}

	public:

		Image(const Image& other)
			: m_image_data{ nullptr, [](Byte* image_data) { delete[] image_data; } },
			m_width{ other.m_width }, m_height{ other.m_height },
			m_image_channels{ other.m_image_channels } { Copy_Data(other); }
		Image(Image&& other) noexcept
			: m_image_data{ std::move(other.m_image_data) },
			m_width{ other.m_width }, m_height{ other.m_height },
			m_image_channels{ other.m_image_channels } { }

		Image& operator=(const Image& other)
		{
			if (this == &other)
				return *this;
			Free();
			Copy_Data(other);
			m_width = other.m_width;
			m_height = other.m_height;
			m_image_channels = other.m_image_channels;
			return *this;
		}
		Image& operator=(Image&& other) noexcept
		{
			if (this == &other)
				return *this;
			Free();
			m_image_data = std::move(other.m_image_data);
			m_width = other.m_width;
			m_height = other.m_height;
			m_image_channels = other.m_image_channels;
			return *this;
		}

		/**
		 * @brief Loads the image stored in the file of the given name.
		 * 
		 * @param image_channels - The image channels that the image data will be loaded with. If
		 * file is 'GIF' then this is ignored and the data is loaded with RGBA channels.
		 * @param flip_vertical - Whether or not to load the data in a flipped state.
		 */
		Image(const std::string& filename, enums::Image_Channels image_channels = 
			enums::Image_Channels::Largest, bool flip_vertical = false);
		/**
		 * @brief Loads the image stored in the file of the given name.
		 *
		 * @param image_channels - The image channels that the image data will be loaded with. If
		 * file is 'GIF' then this is ignored and the data is loaded with RGBA channels.
		 * @param flip_vertical - Whether or not to load the data in a flipped state.
		 */
		void Reload(const std::string& filename, enums::Image_Channels image_channels =
			enums::Image_Channels::Largest, bool flip_vertical = false);

		/**
		 * @brief Frees the previously stored image.
		 */
		void Free();

		/**
		 * @brief Gets a pointer to the image data.
		 */
		auto Get_Data() { return m_image_data.get(); }
		/**
		 * @brief The width of the image.
		 */
		auto Width() const { return m_width; }
		/**
		 * @brief The height of the image.
		 */	
		auto Height() const { return m_height; }
		/**
		 * @brief The channels that the image data contains.
		 */
		auto Channels() const { return m_image_channels; }
		/**
		 * @brief The total size of the image data.
		 */
		auto Size() const { return m_width * m_height * (*m_image_channels); }

#if TILIA_UNIT_TESTS == 1

		/**
		 * @brief Unit test for Image.
		 */
		static void Test();

#endif // TILIA_UNIT_TESTS == 1

	private:
		static void Free_Image(Byte* image_data);
		// The data of the image
		std::unique_ptr<Byte[], void (*)(Byte*)> m_image_data{ nullptr, nullptr };
		// Dimensions of the image
		std::int32_t
			m_width{},
			m_height{};
		// The channels contained in the image data
		enums::Image_Channels m_image_channels{};
	}; // Image

} // tilia

#endif // TILIA_IMAGE_HPP