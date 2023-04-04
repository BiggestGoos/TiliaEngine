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

namespace tilia
{

	namespace enums
	{
		enum class Image_Format
		{
			Red		  = 0x0000,
			Red_Green = 0x0001,
			RGB       = 0x0002,
			BGR		  = 0x0003,
			RGBA	  = 0x0004,
			BGRA	  = 0x0005
		};
	}

	class Image
	{
	private:

		using Byte = uint8_t;

	public:



	private:

		Byte* m_texture_data{};

		std::int32_t m_width{}, m_height{};
		std::int32_t m_channel_count{};

	};

}

#endif // TILIA_IMAGE_HPP