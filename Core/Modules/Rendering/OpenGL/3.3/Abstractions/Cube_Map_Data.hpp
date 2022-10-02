/**************************************************************************************************
 * @file   Cube_Map_Data.hpp
 * 
 * @brief  Contains a class which holds the data of the Cube_Map class.
 * 
 * @define TILIA_CUBE_MAP_DATA_HPP
 * 
 * Standard:
 * <array>
 * <memory>
 * <string>
 * 
 * Tilia:
 * "Enums.hpp"
 * 
 * @author Gustav Fagerlind
 * @date   06/09/2022
 *************************************************************************************************/

#ifndef TILIA_CUBE_MAP_DATA_HPP
#define TILIA_CUBE_MAP_DATA_HPP

 // Standard
#include <array>
#include <memory>
#include <string>

// Tilia
#include "Core/Values/OpenGL/3.3/Enums.hpp"

namespace tilia {

	namespace gfx {
        
        /**
         * @brief A struct which holds information for the Cube_Map class.
         */
        class Cube_Map_Data
        {
        public:

            /**
             * @brief A struct which holds data for each side of the Cube_Map class.
             */
            struct Cube_Side {
                // The file path.
                std::string                                 file_path{};
                // The texture data.
                std::unique_ptr<uint8_t[]>                  texture_data{};
                // The color format.
                enums::Color_Format	                        color_format{ enums::Color_Format::RGBA8 };
                // The color format of the data.
                enums::Data_Color_Format	                data_color_format{ enums::Data_Color_Format::None };
                
            };
            // The data for all of the sides of a cube map.
            std::array<Cube_Side, *enums::Misc::Cube_Sides> sides;
            // The size in texels of each side.
            std::int32_t							        size{};
            // The filtering mode for minification.
            enums::Filter_Mode				                filter_min{ enums::Filter_Mode::Point };
            // The filtering mode for magnification.
            enums::Filter_Mode				                filter_mag{ enums::Filter_Mode::Point };
            // The wrapping mode for the horizontal/x axis.
            enums::Wrap_Mode				                wrap_s{ enums::Wrap_Mode::Repeat };
            // The wrapping mode for the vertical/y axis.
            enums::Wrap_Mode				                wrap_t{ enums::Wrap_Mode::Repeat };
            // The wrapping mode for the depth/z axis.
            enums::Wrap_Mode				                wrap_r{ enums::Wrap_Mode::Repeat };

            /**
             * @brief Copy-assignment. Shallow copies everything except for the data of all the sides. It will instead be reset to a new pointer and then have the data copied over.
             *
             * @param other - The Cube_Map_Data for which to copy from.
             */
            Cube_Map_Data& operator=(const Cube_Map_Data& other) noexcept;

            /**
             * @brief Move-assignment. Copies everything except for the data of all the sides. It will instead be reset to the pointer of the other data.
             *
             * @param other - The Cube_Map_Data for which to move from.
             */
            Cube_Map_Data& operator=(Cube_Map_Data&& other) noexcept;

            Cube_Map_Data() = default;

            /**
             * @brief Simple copy-constructor which calls copy-assignment.
             *
             * @param other - The Cube_Map_Data for which to copy from.
             */
            Cube_Map_Data(const Cube_Map_Data& other) noexcept;

            /**
             * @brief Simple move-constructor which calls move-assignment.
             *
             * @param other - The Cube_Map_Data for which to move from.
             */
            Cube_Map_Data(Cube_Map_Data&& other) noexcept;

            /**
             * @brief Reloads the texture data of the side with the given index with the path of the index.
             *
             * @param index - The index of the side to reload the data of.
             */
            void Reload(const std::size_t& index);

            /**
             * @brief Reloads the texture data of all of the sides with the paths of all the sides.
             */
            void Reload();

            /**
             * @brief Copies the data from the given texture data to the side with the given index.
             *
             * @param index        - The index of the side for which to set the data of.
             * @param texture_data - The texture data for which to copy.
             * @param byte_count   - The amount of bytes for which to allocate and copy.
             */
            void Copy_Data(const std::size_t& index, uint8_t* texture_data, uint32_t byte_count);

        }; // Cube_Map_Data
        
	} // gfx

} // tilia

#endif // TILIA_CUBE_MAP_DATA_HPP