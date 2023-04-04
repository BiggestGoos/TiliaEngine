// Vendor
#include "vendor/stb_image/include/stb_image/stb_image.h"

// Tilia
#include "Image.hpp"

tilia::Image::Image(const std::string& filename, bool flip_vertical)
{
	stbi_set_flip_vertically_on_load(flip_vertical);

	auto image_data{ stbi_load(filename.c_str(), &m_width, &m_height, &m_channel_count, 0) };

	m_image_data.reset(image_data);

}

void tilia::Image::Free_Image(Byte* image_data)
{
	stbi_image_free(image_data);
}
