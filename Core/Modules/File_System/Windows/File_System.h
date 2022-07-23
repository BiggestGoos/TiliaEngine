/*****************************************************************//**
 * @file   File_System.h
 * 
 * @brief  Holds a class which will do all of the saving and loading 
 *		   of files.
 * 
 * @define TILIA_FILE_SYSTEM_H
 * 
 * Standard:
 * <cstdint>
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
         *		  files.
         */
        class File_System {
        public:

            /**
             * @brief Loads in the data from an image file. Eg. .png.
             * 
             * @param file_path				 - The path of the image to
             *								   load.
             * @param width					 - The width of the loaded
             *								   image.
             * @param height                 - The height of the loaded
             *								   image.
             * @param color_channels		 - The number of color 
             *								   channels of the image.
             * @param desired_color_channels - The desired amount of 
             *								   color channels to be 
             *								   loaded.
             *              
             * @return						   The data of the image in
             *                                 bytes.
             * 
             * @exception Guarantee: Strong
             * @exception Reasons:
             * @exception Loaded data is null.
             * @exception Loaded witdh, height or number of color
             *            channels is equel to zero.
             */
            std::uint8_t* Load_Image(const char* file_path, std::int32_t& width, std::int32_t& height, std::int32_t& color_channels,
                                const std::int32_t& desired_color_channels, const bool& flip_image_y);

        }; // File_System

    } // utils

} // tilia

#endif // TILIA_FILE_SYSTEM_H