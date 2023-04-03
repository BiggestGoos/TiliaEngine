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