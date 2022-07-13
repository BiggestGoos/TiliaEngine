/*****************************************************************//**
 * @file   File_System.h
 * @brief  Holds a class which will do all of the saving and loading 
 *		   of files.
 * 
 * @define TILIA_FILE_SYSTEM_H
 * 
 * Standard:
 * @include <cstdint>
 * 
 * @author Gustav Fagerlind
 * @date   12/07/2022
 *********************************************************************/

#ifndef TILIA_FILE_SYSTEM_H
#define TILIA_FILE_SYSTEM_H

// Standard
#include <cstdint>

namespace tilia {

	namespace utils {

		/**
		 * @brief A class which holds all of the functions for loading 
		 * files. This is because of differences between different 
		 * platforms.
		 */
		class File_System {
		public:

			/**
			 * @brief Loads in the data from an image file. 
			 * 
			 * @param file_path - The path of the image to load.
			 * @param width - The width of the loaded image.
			 * @param height - The height of the loaded image.
			 * @param color_channels - The number of color channels of the image.
			 * @param desired_color_channels - The desired amount of color channels to be loaded.
			 * 
			 * @return The data of the image in bytes. 
			 * 
			 * @exception Throws exception if file not loaded properly.
			 * @exception Exception params:
			 * If the loaded data is null or if the width, height or color channels is zero.
			 */
			uint8_t* Load_Image(const char* file_path, int& width, int& height, int& color_channels, 
				const int& desired_color_channels, const bool& flip_image_y);

		private:

		};

		extern File_System file_system{};

	}

}

#endif