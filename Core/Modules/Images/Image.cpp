// Vendor
#include "vendor/stb_image/include/stb_image/stb_image.h"

// Tilia
#include "Image.hpp"
#include TILIA_TILIA_EXCEPTION_INCLUDE

static inline bool Is_Gif(const std::string& filename)
{
	constexpr auto gif_length{ 3 };
	const auto extension{ filename.substr(filename.size() - gif_length) };
	return (extension == "gif");
}

tilia::Image::Image(const std::string& filename, enums::Image_Channels image_channels, 
	bool flip_vertical)
	: m_image_data{ nullptr, Free_Image }
{
	Reload(filename, image_channels, flip_vertical);
}

void tilia::Image::Reload(const std::string& filename, enums::Image_Channels image_channels, 
	bool flip_vertical)
{
	Free();
	stbi_set_flip_vertically_on_load(flip_vertical);
	std::int32_t channel_count{};
	m_image_data = { 
		stbi_load(filename.c_str(), &m_width, &m_height, 
		&channel_count, *image_channels), Free_Image };
	// https://github.com/nothings/stb/blob/master/stb_image.h (135)
	// 'GIF always returns *comp=4'
	if (Is_Gif(filename))
	{
		m_image_channels = enums::Image_Channels::RGBA;
	}
	else if (image_channels != enums::Image_Channels::Largest)
	{
		m_image_channels = image_channels;
	}
	else
	{
		m_image_channels = static_cast<enums::Image_Channels>(channel_count);
	}
	if (m_image_data == nullptr)
	{
		const std::string message{ stbi_failure_reason() };
		throw utils::Tilia_Exception{ { TILIA_LOCATION,
			"Failed to load image: ",
			"\nFilename: ", filename,
			"\nImage channels: ", *image_channels,
			"\nMessage: ", message } };
	}
}

void tilia::Image::Free()
{
	m_image_data.reset();
	m_width = 0;
	m_height = 0;
	m_image_channels = enums::Image_Channels::Largest;
}

void tilia::Image::Free_Image(Byte* image_data)
{
	stbi_image_free(image_data);
}

#if TILIA_UNIT_TESTS == 1

// Vendor
#include "vendor/Catch2/Catch2.hpp"

void tilia::Image::Test()
{

	const auto filename_0{ "res/textures/spaceInvader.png" };
	const auto filename_1{ "res/textures/spaceInvader.jpg" };
	const auto filename_2{ "res/textures/spaceInvader.gif" };

	// Test loading PNG

	Image image_0{ filename_0, enums::Image_Channels::Largest };

	REQUIRE(image_0.Width() > 0);
	REQUIRE(image_0.Height() > 0);
	REQUIRE(image_0.Channels() == enums::Image_Channels::RGBA);
	REQUIRE(image_0.Get_Data() != nullptr);

		// Test with different image channels

			// Gray:

	image_0.Reload(filename_0, enums::Image_Channels::Gray);

	REQUIRE(image_0.Channels() == enums::Image_Channels::Gray);
	REQUIRE(image_0.Get_Data() != nullptr);

			// Gray alpha:

	image_0.Reload(filename_0, enums::Image_Channels::Gray_Alpha);

	REQUIRE(image_0.Channels() == enums::Image_Channels::Gray_Alpha);
	REQUIRE(image_0.Get_Data() != nullptr);

			// RGB:

	image_0.Reload(filename_0, enums::Image_Channels::RGB);

	REQUIRE(image_0.Channels() == enums::Image_Channels::RGB);
	REQUIRE(image_0.Get_Data() != nullptr);

			// RGBA:

	image_0.Reload(filename_0, enums::Image_Channels::RGBA);

	REQUIRE(image_0.Channels() == enums::Image_Channels::RGBA);
	REQUIRE(image_0.Get_Data() != nullptr);

	// Test loading JPG

	Image image_1{ filename_1, enums::Image_Channels::Largest };

	REQUIRE(image_1.Width() > 0);
	REQUIRE(image_1.Height() > 0);
	REQUIRE(image_1.Channels() == enums::Image_Channels::RGB);
	REQUIRE(image_1.Get_Data() != nullptr);

	// Test loading GIF

	Image image_2{ filename_2, enums::Image_Channels::Largest };

	REQUIRE(image_2.Width() > 0);
	REQUIRE(image_2.Height() > 0);
	REQUIRE(image_2.Channels() == enums::Image_Channels::RGBA);
	REQUIRE(image_2.Get_Data() != nullptr);

		// Loading GIF always gives RGBA as image channels

			// Gray:

	image_2.Reload(filename_2, enums::Image_Channels::Gray);

	REQUIRE(image_2.Channels() == enums::Image_Channels::RGBA);
	REQUIRE(image_2.Get_Data() != nullptr);

			// Gray alpha:

	image_2.Reload(filename_2, enums::Image_Channels::Gray_Alpha);

	REQUIRE(image_2.Channels() == enums::Image_Channels::RGBA);
	REQUIRE(image_2.Get_Data() != nullptr);

			// RGB:

	image_2.Reload(filename_2, enums::Image_Channels::RGB);

	REQUIRE(image_2.Channels() == enums::Image_Channels::RGBA);
	REQUIRE(image_2.Get_Data() != nullptr);

			// RGBA:

	image_2.Reload(filename_2, enums::Image_Channels::RGBA);

	REQUIRE(image_2.Channels() == enums::Image_Channels::RGBA);
	REQUIRE(image_2.Get_Data() != nullptr);

	// Test freeing

	image_0.Reload(filename_0);

	REQUIRE(image_0.Get_Data() != nullptr);
	REQUIRE(image_0.Width() > 0);
	REQUIRE(image_0.Height() > 0);

	image_0.Free();

	REQUIRE(image_0.Get_Data() == nullptr);
	REQUIRE(image_0.Width() == 0);
	REQUIRE(image_0.Height() == 0);

	// Test copy-constructor

	image_0.Reload(filename_0);

	Image image_3{ image_0 };

	REQUIRE(image_3.Width() == image_0.Width());
	REQUIRE(image_3.Height() == image_0.Height());
	REQUIRE(image_3.Channels() == image_0.Channels());

	const auto size_0{ image_0.Width() * image_0.Height() * (*image_0.Channels()) };

	REQUIRE(std::memcmp(image_3.Get_Data(), image_0.Get_Data(), size_0) == 0);

	// Test move-constructor

	image_0.Reload(filename_0);

	Image image_4{ std::move(image_0) };

	image_0.Reload(filename_0);

	REQUIRE(image_4.Width() == image_0.Width());
	REQUIRE(image_4.Height() == image_0.Height());
	REQUIRE(image_4.Channels() == image_0.Channels());

	const auto size_1{ image_0.Width() * image_0.Height() * (*image_0.Channels()) };

	REQUIRE(std::memcmp(image_4.Get_Data(), image_0.Get_Data(), size_1) == 0);

	// Test copy-assignment

	image_0.Reload(filename_0);

	image_3.Free();

	image_3 = image_0;

	REQUIRE(image_3.Width() == image_0.Width());
	REQUIRE(image_3.Height() == image_0.Height());
	REQUIRE(image_3.Channels() == image_0.Channels());

	const auto size_2{ image_0.Width() * image_0.Height() * (*image_0.Channels()) };

	REQUIRE(std::memcmp(image_3.Get_Data(), image_0.Get_Data(), size_0) == 0);

	// Test move-assignment

	image_0.Reload(filename_0);

	image_4.Free();

	image_4 = std::move(image_0);

	image_0.Reload(filename_0);

	REQUIRE(image_4.Width() == image_0.Width());
	REQUIRE(image_4.Height() == image_0.Height());
	REQUIRE(image_4.Channels() == image_0.Channels());

	const auto size_3{ image_0.Width() * image_0.Height() * (*image_0.Channels()) };

	REQUIRE(std::memcmp(image_4.Get_Data(), image_0.Get_Data(), size_0) == 0);

}

#endif // TILIA_UNIT_TESTS == 1