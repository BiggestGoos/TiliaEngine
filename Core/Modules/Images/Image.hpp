/**************************************************************************************************
 * @file   Image.hpp
 * 
 * @brief  
 * 
 * @author Gustav Fagerlind
 * @date   03/04/2023
 *************************************************************************************************/

#ifndef TILIA_IMAGE_HPP
#define TILIA_IMAGE_HPP

// Standard
#include <memory>
#include <string>

namespace tilia
{

	namespace enums
	{
		enum class Image_Channels
		{
			Gray	   = 0x0000,
			Gray_Alpha = 0x0001,
			RGB		   = 0x0002,
			RGBA	   = 0x0003
		}; // Image_Channels
	} // enums

	class Image
	{
	private:

		using Byte = uint8_t;

	public:

		Image(const std::string& filename, bool flip_vertical = false);

		auto Get() { return m_image_data.get(); }

		auto Width() { return m_width; }
		auto Height() { return m_height; }
		auto Channel_Count() { return m_channel_count; }

	private:

		static void Free_Image(Byte* image_data);

		std::unique_ptr<Byte[], void (*)(Byte*)> m_image_data{ nullptr, Free_Image };

		std::int32_t 
			m_width{}, 
			m_height{}, 
			m_channel_count{};

	}; // Image

} // tilia

#endif // TILIA_IMAGE_HPP