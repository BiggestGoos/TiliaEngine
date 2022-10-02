// Vendor
#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image/include/stb_image/stb_image.h"

// Standard
#include <fstream>

// Tilia
#include "Core/Modules/File_System/Windows/File_System.hpp"
#include "Core/Modules/Exceptions/Tilia_Exception.hpp"

// Global File_System
tilia::utils::File_System file_system{};

std::uint8_t* tilia::utils::File_System::Load_Image(const std::string& file_path, std::int32_t& width, std::int32_t& height, std::int32_t& color_channels,
											   const std::int32_t& desired_color_channels, const bool& flip_image_y)
{
	// Stores the vertical flipping flag if it needs to be reset.
	std::int32_t temp_flip_flag{ stbi__vertically_flip_on_load_global };
	// Sets the vertical flipping flag
	stbi_set_flip_vertically_on_load(flip_image_y);

	// Loads the data along with the dimensions and amount of color channels.
	std::uint8_t* data{
		stbi_load(file_path.c_str(), &width, &height, &color_channels, desired_color_channels)
	};

	// Checks if any of the loaded data is invalid.
	if (!data || !width || !height || !color_channels)
	{
		// Resets vertical flipping flag.
		stbi_set_flip_vertically_on_load(temp_flip_flag);
		// If any data was loaded then it frees it.
		if (data)
			stbi_image_free(data);
		// Creates exception with some information.
		utils::Tilia_Exception e{ LOCATION };
		e.Add_Message(
			"Image file did not load properly"
			"\n>>> Path: %v"
			"\n>>> Witdh: %v"
			"\n>>> Height: %v"
			"\n>>> Color channels: %v"
			)(file_path)(width)(height)(color_channels);

		throw e;
	}

	return data;
}

std::string tilia::utils::File_System::Load_File(const std::string& file_path)
{
	
	std::ifstream file{};

	std::string data{};

	// Sets exceptions
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{

		file.open(file_path);

		// Gets the data from the file stream
		data = (std::stringstream() << file.rdbuf()).str();

		file.close();

	}
	catch (std::exception& e)
	{

		utils::Tilia_Exception t_e{ LOCATION };

		t_e.Add_Message("File did not load properly"
			"\n>>> Path: %v"
			"\n>>> Message: %v"
			)(file_path)(e.what());

		throw t_e;

	}

	return std::move(data);

}
