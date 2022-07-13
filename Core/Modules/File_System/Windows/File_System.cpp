/*****************************************************************//**
 * @file   File_System.cpp
 * @brief  Holds definitions of functions and such for the @include 
 *		   "File_System.h" @File_System class.
 * 
 * Dependencies:
 * @include "dependencies/stb_image/include/stb_image/stb_image.h"
 * 
 * Standard:
 * @include <string>
 * @include <sstream>
 * 
 * Headers:
 * @include "Core/Modules/File_System/Windows/File_System.h"
 * @include "Core/Modules/Exceptions/Tilia_Exception.h"
 * 
 * @author Gustav Fagerlind
 * @date   12/07/2022
 *********************************************************************/

// Dependencies
#include "dependencies/stb_image/include/stb_image/stb_image.h"

// Standard
#include <string>
#include <sstream>

// Headers
#include "Core/Modules/File_System/Windows/File_System.h"
#include "Core/Modules/Exceptions/Tilia_Exception.h"

/**
 * Sets the vertical flipping flag. Loads the data along with the dimensions.
 * Then checks for loading errors.
 */
uint8_t* tilia::utils::File_System::Load_Image(const char* file_path, int& width, int& height, int& color_channels, 
	const int& desired_color_channels, const bool& flip_image_y)
{
	stbi_set_flip_vertically_on_load(flip_image_y);

	uint8_t* data{
		stbi_load(file_path, &width, &height, &color_channels, desired_color_channels)
	};

	if (!data || !width || !height || !color_channels)
	{
		if (data)
			stbi_image_free(data);
		std::stringstream ss{};
		ss << "Image file did not load properly" <<
		    "\nPath: " << file_path <<
			"\nWitdh: " << width <<
			"\nHeight: " << height <<
			"\nColor channels: " << color_channels;
		throw utils::Tilia_Exception{ ss, LOCATION };
	}

	return data;
}
