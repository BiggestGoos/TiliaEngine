/*****************************************************************//**
 * @file   Cube_Map.h
 * 
 * @brief  Declares and defines a class called Cube_Map deriving from
 *		   Texture in "Texture.h", which works as an abstraction for an
 *         openGL cube map texture as well as a struct called 
 *         Cube_Map_Data that holds some information about the class.
 * 
 * @define TILIA_CUBE_MAP_H
 * 
 * Standard:
 * <array>
 * <memory>
 * 
 * Tilia:
 * "Texture.h"
 * 
 * @author Gustav Fagerlind
 * @date   04/06/2022
 *********************************************************************/

// Standard
#include <array>
#include <memory>

// Tilia
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Texture.h"

#ifndef TILIA_CUBE_MAP_H
#define TILIA_CUBE_MAP_H

namespace tilia {

    namespace render {

        /**
         * @brief A struct which holds information for the Cube_Map
         *        class.
         */
        struct Cube_Map_Data
        {
            /**
             * @brief A struct which holds data for each side of a cube
             *        map.
             */
            struct Cube_Side {
                // The file path
                std::string                                 file_path{};
                // The texture data.
                std::unique_ptr<uint8_t[]>                  texture_data{};
                // The color format.
                enums::Color_Format	                        color_format{ enums::Color_Format::RGBA8 };
                // The loaded color format.
                enums::Color_Format	                        load_color_format{ enums::Color_Format::None };
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
             * @brief Assignment operator. Shallow copies everything 
             *        except for the data of all the sides which 
             *        will get reset to nullptr.
             */
            const Cube_Map_Data& operator=(const Cube_Map_Data& other);

        }; // Cube_Map_Def

        /**
         * @brief A class deriving from the "Texture.h" Texture class
         *        which works as an abstraction for an openGL texture.
         *        Also contains a Cube_Map_Data object.
         */
        class Cube_Map : public Texture {
        public:

            /**
             * @brief The default constructor which generates an openGL
             *        cube map and sets the texture id.
             * 
             * @exception Guarantee: Strong
             * @exception Reasons: 
             * @exception OpenGL fails to generate a new cube map.
             */
            Cube_Map();

            inline operator Cube_Map_Data&()             { return m_cube_map_data; }
            inline operator const Cube_Map_Data&() const { return m_cube_map_data; }

            /**
             * @brief Sets the Cube_Map_Data of the Cube_Map. If there
             *		  already is any texture data in the given 
             *        Cube_Map_Data then it attempts to copy that data.
             *        If not then it attempts to load the texture data
             *        from the image at the file paths.
             *
             * @param cube_map_data - The Cube_Map_Data for which to
             *                        set this Cube_Map's Cube_Map_Data
             *                        to.
             *
             * @exception Guarantee:
             * @exception Reasons:
             * @exception The data is not copied or loaded properly.
             */
            void Set(const Cube_Map_Data& cube_map_data);

            /**
             * @brief Sets the Cube_Map_Data's texture paths to have
             *        the given texture paths and then sets the texture
             *        using those paths.
             *
             * @param texture_paths - The paths of the textures to 
             *                        load.
             * 
             * @exception Guarantee:
             * @exception Reasons:
             * @exception The data is not copied or loaded properly.
             */
            void Set(const std::array<std::string, 6>& texture_paths);

            /**
             * @brief Sets the filtering mode for the given filtering
             *        size.
             *
             * @param filter_size - The size of filtering for which to
             *                      set the mode of.
             * @param filter_mode - The mode of filtreing for which to
             *                      use for the size.
             */
            void Set(const enums::Filter_Size& filter_size, const enums::Filter_Mode& filter_mode) override;

            /**
             * @brief Set wrapping for the given side.
             *
             * @param wrap_side - The side of which to set wrapping
             *                    for.
             * @param wrap_mode - The wrapping mode to set for the
             *                    given side.
             */
            void Set(const enums::Wrap_Sides& wrap_side, const enums::Wrap_Mode& wrap_mode) override;

            /**
             * @brief Generates all mipmap levels for the cube map.
             *
             * @exception Guarantee:
             * @exception Reasons:
             */
            void Generate_Mipmaps() override;

            /**
             * @brief Returns the Cube_Map_Data of this Cube_Map.
             *
             * @return The Cube_Map_Data of this Cube_Map.
             */
            inline const Cube_Map_Data& Get_Data() const { return m_cube_map_data; }

            /**
             * @brief Returns the size of this Cube_Map.
             *
             * @return The size of this Cube_Map.
             */
            inline const std::int32_t& Get_Size() const { return m_cube_map_data.size; }

        private:

            // The info pertaining to this Texture
            Cube_Map_Data m_cube_map_data;

        }; // Cube_Map

    } // render

} // tilia

#endif // TILIA_CUBE_MAP_H