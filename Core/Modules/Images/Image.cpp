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
	enums::Image_Data_Type data_type, bool flip_vertical, float gamma)
	: m_image_data{ nullptr, Free_Image }
{
	Reload(filename, image_channels, data_type, flip_vertical, gamma);
}

void tilia::Image::Reload(const std::string& filename, enums::Image_Channels image_channels,
	enums::Image_Data_Type data_type, bool flip_vertical, float gamma)
{
	Free();
	stbi_set_flip_vertically_on_load(flip_vertical);
	stbi_ldr_to_hdr_gamma(gamma);
	std::int32_t channel_count{};
	if (data_type == enums::Image_Data_Type::Unsigned_Byte)
	{
		m_image_data = {
			stbi_load(filename.c_str(), &m_width, &m_height,
			&channel_count, *image_channels), Free_Image };
	}
	else if (data_type == enums::Image_Data_Type::Float)
	{
		m_image_data = { static_cast<Byte*>(
			static_cast<void*>(stbi_loadf(filename.c_str(), &m_width, &m_height,
			&channel_count, *image_channels))), Free_Image };
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

	m_data_type = data_type;

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
}

void tilia::Image::Free()
{
	m_image_data.reset();
	m_width = 0;
	m_height = 0;
	m_image_channels = enums::Image_Channels::Largest;
	m_data_type = enums::Image_Data_Type::Unsigned_Byte;
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
	REQUIRE(image_0.Data_Type() == enums::Image_Data_Type::Unsigned_Byte);
	REQUIRE(image_0.Get_Data() != nullptr);

		// Test with different image channels

			// Grey:

	image_0.Reload(filename_0, enums::Image_Channels::Grey);

	REQUIRE(image_0.Channels() == enums::Image_Channels::Grey);
	REQUIRE(image_0.Get_Data() != nullptr);

			// Grey alpha:

	image_0.Reload(filename_0, enums::Image_Channels::Grey_Alpha);

	REQUIRE(image_0.Channels() == enums::Image_Channels::Grey_Alpha);
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

			// Grey:

	image_2.Reload(filename_2, enums::Image_Channels::Grey);

	REQUIRE(image_2.Channels() == enums::Image_Channels::RGBA);
	REQUIRE(image_2.Get_Data() != nullptr);

			// Grey alpha:

	image_2.Reload(filename_2, enums::Image_Channels::Grey_Alpha);

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

	// Test loading with float as data type

	image_0.Reload(filename_0, enums::Image_Channels::Largest, enums::Image_Data_Type::Float);

	REQUIRE(image_0.Width() > 0);
	REQUIRE(image_0.Height() > 0);
	REQUIRE(image_0.Channels() == enums::Image_Channels::RGBA);
	REQUIRE(image_0.Data_Type() == enums::Image_Data_Type::Float);
	REQUIRE(image_0.Get_Data() != nullptr);

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

	REQUIRE(std::memcmp(image_3.Get_Data(), image_0.Get_Data(), size_2) == 0);

	// Test move-assignment

	image_0.Reload(filename_0);

	image_4.Free();

	image_4 = std::move(image_0);

	image_0.Reload(filename_0);

	REQUIRE(image_4.Width() == image_0.Width());
	REQUIRE(image_4.Height() == image_0.Height());
	REQUIRE(image_4.Channels() == image_0.Channels());

	const auto size_3{ image_0.Width() * image_0.Height() * (*image_0.Channels()) };

	REQUIRE(std::memcmp(image_4.Get_Data(), image_0.Get_Data(), size_3) == 0);

	// Test copy-constructor taking raw pixel data and dimensions

	image_0.Reload(filename_0);

	Image image_5{ image_0.Get_Data(), image_0.Width(), image_0.Height(), image_0.Channels(), 
		image_0.Data_Type() };

	REQUIRE(image_5.Width() == image_0.Width());
	REQUIRE(image_5.Height() == image_0.Height());
	REQUIRE(image_5.Channels() == image_0.Channels());

	const auto size_4{ image_0.Width() * image_0.Height() * (*image_0.Channels()) };

	REQUIRE(std::memcmp(image_5.Get_Data(), image_0.Get_Data(), size_4) == 0);

	// Test move-constructor taking raw pixel data and dimensions

	image_0.Reload(filename_0);

	const auto size_5{ image_0.Size() };
	
	Data_Ptr image_data_0{ new Byte[size_5], [](Byte* image_data) { delete[] image_data; } };
	std::copy(image_0.Get_Data(), image_0.Get_Data() + size_5, image_data_0.get());

	Image image_6{ std::move(image_data_0), image_0.Width(), image_0.Height(), 
		image_0.Channels(), image_0.Data_Type() };

	REQUIRE(image_6.Width() == image_0.Width());
	REQUIRE(image_6.Height() == image_0.Height());
	REQUIRE(image_6.Channels() == image_0.Channels());

	REQUIRE(std::memcmp(image_6.Get_Data(), image_0.Get_Data(), size_5) == 0);

	// Test '.Size' == width * height * channels

	image_0.Reload(filename_0);

	const auto size_6{ image_0.Width() * image_0.Height() * (*image_0.Channels()) };

	REQUIRE(image_0.Size() == size_6);

	// Test comparison operators

		// ==

			// Both valid

	image_0.Reload(filename_0, enums::Image_Channels::RGBA, enums::Image_Data_Type::Unsigned_Byte, true);

	image_1.Reload(filename_0, enums::Image_Channels::RGBA, enums::Image_Data_Type::Unsigned_Byte, true);

	REQUIRE(image_0 == image_1);

			// Both invalid

	image_0.Free();

	image_1.Free();

	REQUIRE(image_0 == image_1);

		// !=

			// Both valid

	image_0.Reload(filename_0, enums::Image_Channels::RGBA, enums::Image_Data_Type::Unsigned_Byte, true);

	image_1.Reload(filename_0, enums::Image_Channels::RGBA, enums::Image_Data_Type::Unsigned_Byte, false);

	REQUIRE(image_0 != image_1);

			// One invalid

	image_0.Reload(filename_0, enums::Image_Channels::RGBA, enums::Image_Data_Type::Unsigned_Byte, true);

	image_1.Free();

	REQUIRE(image_0 != image_1);

	// Test '.Free' resulst in same as '= {}'

	image_0.Reload(filename_0);

	image_1.Reload(filename_0);

	image_0.Free();

	image_1 = {};

	REQUIRE(image_0 == image_1);
	
	
}

#endif // TILIA_UNIT_TESTS == 1