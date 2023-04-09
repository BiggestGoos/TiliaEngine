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
			Grey	   = 0x0001,
			Grey_Alpha = 0x0002,
			RGB		   = 0x0003,
			RGBA	   = 0x0004
		}; // Image_Channels
		// The data type to use to represent the image data.
		enum class Image_Data_Type
		{
			Unsigned_Byte = sizeof(unsigned char),
			Float		  = sizeof(float)
		}; // Image_Data_Type
	} // enums
	
	/**
	 * @brief A class for image loading. Can be copied and moved for both construction and
	 * assignment.
	 */
	class Image
	{
	private:

		using Byte = std::uint8_t;

		using Data_Ptr = std::unique_ptr<Byte[], void (*)(Byte*)>;

	public:

		/**
		 * @brief Loads the image stored in the file of the given name.
		 * 
		 * @param image_channels - The image channels that the image data will be loaded with. If
		 * file is 'GIF' then this is ignored and the data is loaded with RGBA channels.
		 * @param data_type - The type of data for the image to be loaded as.
		 * @param flip_vertical - Whether or not to load the data in a flipped state.
		 * @param gamma - The gamma to load the image with. Only works on images loaded as float.
		 */
		Image(const std::string& filename, enums::Image_Channels image_channels =
			enums::Image_Channels::Largest, enums::Image_Data_Type data_type = 
			enums::Image_Data_Type::Unsigned_Byte, bool flip_vertical = false, float gamma = 1.0f);
		/**
		 * @brief Loads the image stored in the file of the given name.
		 *
		 * @param image_channels - The image channels that the image data will be loaded with. If
		 * file is 'GIF' then this is ignored and the data is loaded with RGBA channels.
		 * @param data_type - The type of data for the image to be loaded as.
		 * @param flip_vertical - Whether or not to load the data in a flipped state.
		 * @param gamma - The gamma to load the image with. Only works on images loaded as float.
		 */
		void Reload(const std::string& filename, enums::Image_Channels image_channels =
			enums::Image_Channels::Largest, enums::Image_Data_Type data_type =
			enums::Image_Data_Type::Unsigned_Byte, bool flip_vertical = false, float gamma = 1.0f);

		/**
		 * @brief Restores image to default.
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
		 * @brief The data type that the image stores the image data as.
		 */
		auto Data_Type() const { return m_data_type; }
		/**
		 * @brief The total size of the image data in bytes.
		 */
		auto Size() const { return m_width * m_height * (*m_image_channels) * (*m_data_type); }

#if TILIA_UNIT_TESTS == 1

		/**
		 * @brief Unit test for Image.
		 */
		static void Test();

#endif // TILIA_UNIT_TESTS == 1

	private:
		static void Free_Image(Byte* image_data);
		// The data of the image
		Data_Ptr m_image_data{ nullptr, nullptr };
		// Dimensions of the image
		std::int32_t
			m_width{ 0 },
			m_height{ 0 };
		// The channels contained in the image data
		enums::Image_Channels m_image_channels{ enums::Image_Channels::Largest };
		// The data type the image data is stored as
		enums::Image_Data_Type m_data_type{ enums::Image_Data_Type::Unsigned_Byte };

		/**
		 * @brief Copies the given data. Uses the size of member variabels for width, height and
		 * number of channels.
		 */
		void Copy_Data(Byte* image_data)
		{
			const auto size{ m_width * m_height * (*m_image_channels) * (*m_data_type) };
			// Copies data as well as sets custom destructor to just do 'delete[]' since the data
			// is allocated using new[]
			m_image_data = { new Byte[size], [](Byte* image_data) { delete[] image_data; } };
			std::copy(image_data, image_data + size, m_image_data.get());
		}

	public:

		Image() = default;

		Image(const Image& other)
			: m_image_data{ nullptr, [](Byte* image_data) { delete[] image_data; } },
			m_width{ other.m_width }, m_height{ other.m_height },
			m_image_channels{ other.m_image_channels }, m_data_type{ other.m_data_type }
		{ Copy_Data(other.m_image_data.get()); }
		Image(Image&& other) noexcept
			: m_image_data{ std::move(other.m_image_data) },
			m_width{ other.m_width }, m_height{ other.m_height },
			m_image_channels{ other.m_image_channels }, m_data_type{ other.m_data_type } { }

		Image& operator=(const Image& other)
		{
			if (this == &other)
				return *this;
			Free();
			m_width = other.m_width;
			m_height = other.m_height;
			m_image_channels = other.m_image_channels;
			m_data_type = other.m_data_type;
			Copy_Data(other.m_image_data.get());
			return *this;
		}
		Image& operator=(Image&& other) noexcept
		{
			if (this == &other)
				return *this;
			Free();
			m_width = other.m_width;
			m_height = other.m_height;
			m_image_channels = other.m_image_channels;
			m_data_type = other.m_data_type;
			m_image_data = std::move(other.m_image_data);
			return *this;
		}

		Image(Byte* image_data, std::int32_t width, std::int32_t height,
			enums::Image_Channels image_channels, enums::Image_Data_Type data_type)
			: m_image_data{ nullptr, [](Byte* image_data) { delete[] image_data; } },
			m_width{ width }, m_height{ height }, m_image_channels{ image_channels },
			m_data_type{ data_type }
		{
			Copy_Data(image_data);
		}
		Image(Data_Ptr image_data, std::int32_t width, std::int32_t height,
			enums::Image_Channels image_channels, enums::Image_Data_Type data_type)
			: m_image_data{ std::move(image_data) },
			m_width{ width }, m_height{ height }, m_image_channels{ image_channels },
			m_data_type{ data_type } { }

		friend bool operator==(const Image& lhs, const Image& rhs)
		{
			if (&lhs == &rhs)
				return true;
			if (lhs.m_width != rhs.m_width)
				return false;
			if (lhs.m_height != rhs.m_height)
				return false;
			if (lhs.m_image_channels != rhs.m_image_channels)
				return false;
			if (lhs.m_data_type != rhs.m_data_type)
				return false;
			if ((lhs.m_image_data.get() == nullptr) != (rhs.m_image_data.get() == nullptr))
				return false;
			if (lhs.m_image_data.get() != nullptr && rhs.m_image_data.get() != nullptr && 
				lhs.m_image_data.get() != rhs.m_image_data.get())
				if (std::memcmp(lhs.m_image_data.get(), rhs.m_image_data.get(), lhs.Size()) != 0)
					return false;
			return true;
		}
		friend bool operator!=(const Image& lhs, const Image& rhs)
		{
			if (&lhs == &rhs)
				return false;
			return !(lhs == rhs);
		}

	}; // Image

} // tilia

#endif // TILIA_IMAGE_HPP