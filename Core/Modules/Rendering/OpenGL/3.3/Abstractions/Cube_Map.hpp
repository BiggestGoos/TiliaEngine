/*****************************************************************//*******************************
 * @file   Cube_Map.hpp
 * 
 * @brief  Declares and defines a class called Cube_Map deriving from Texture in "Texture.hpp", 
 *         which works as an abstraction for an openGL cube map texture as well as a struct called
 *         Cube_Map_Data that holds some information about the class.
 * 
 * @define TILIA_CUBE_MAP_HPP
 * 
 * Standard:
 * <array>
 * <memory>
 * 
 * Tilia:
 * "Texture.hpp"
 * 
 * @author Gustav Fagerlind
 * @date   04/06/2022
 *************************************************************************************************/

// Standard
#include <array>
#include <memory>

// Tilia
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Texture.hpp"

#ifndef TILIA_CUBE_MAP_HPP
#define TILIA_CUBE_MAP_HPP

namespace tilia {

    namespace render {
        
        class Cube_Map;

        /**
         * @brief A struct which holds information for the Cube_Map class.
         */
        struct Cube_Map_Data
        {
            /**
             * @brief A struct which holds data for each side of the Cube_Map class.
             */
            struct Cube_Side {
                // The file path
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
             * @brief Copy-assignment. Shallow copies everything except for the data of all the
             *        sides. It will instead be reset to a new pointer and then have the data
             *        copied over.
             * 
             * @param other - The Cube_Map_Data for which to copy from.
             */
            const Cube_Map_Data& operator=(const Cube_Map_Data& other) noexcept;

            /**
             * @brief Move-assignment. Copies everything except for the data of all the
             *        sides. It will instead be reset to the pointer of the other data.
             *
             * @param other - The Cube_Map_Data for which to move from.
             */
            const Cube_Map_Data& operator=(Cube_Map_Data&& other) noexcept;

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
             * @brief Reloads the texture data of the side with the given index with the path of
             *        the index.
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
            void Copy_Data(const std::size_t& index, uint8_t* texture_data, const uint32_t& byte_count);

        }; // Cube_Map_Def

        /**
         * @brief A class deriving from the "Texture.h" Texture class which works as an abstraction
         *        for an openGL cube map. Also contains a Cube_Map_Data object.
         */
        class Cube_Map : public Texture {
            public:

            /**
             * @brief The default constructor which generates an openGL cube map and sets the
             *        texture id.
             * 
             * @exception Guarantee: Strong
             * @exception Reasons: 
             * @exception OpenGL fails to generate a new cube map.
             */
            Cube_Map();

            /**
             * @brief Copy-constructor which calls the default constructor and then copies the
             *        data of the given cube map data.
             * 
             * @param cube_map_data - The cube map data which will be copied.
             */
            Cube_Map(const Cube_Map_Data& cube_map_data) noexcept;

            /**
             * @brief Move-constructor which calls the default constructor and then moves the
             *        data of the given cube map data.
             *
             * @param cube_map_data - The cube map data which will be moved from.
             */
            Cube_Map(Cube_Map_Data&& cube_map_data) noexcept;

            inline operator Cube_Map_Data&()             { return m_cube_map_data; }
            inline operator const Cube_Map_Data&() const { return m_cube_map_data; }
            
            /**
             * @brief Reloads the data for all of the sides of the cube map. 
             * 
             * @exception Guarantee: Basic
             * @exception Reasons:
             * @exception Data is faulty or incorrect.
             */
            void Reload();

            /**
             * @brief Reloads the texture data of the cube map data to the loaded textures from the
             *        contained paths.
             */
            inline void Reload_Cube_Map_Data() {
                m_cube_map_data.Reload();
            }

            /**
             * @brief Generates all mipmap levels for the cube map.
             *
             * @exception Guarantee: Basic
             * @exception Reasons:
             * @exception Any of the side of the cube map is missing texture data.
             * @exception The cube map is not cube complete.
             */
            void Generate_Mipmaps() override;

            /**
             * @brief Copies all of the data from the given cube map data. If told so will also
             *        load the texture data from the paths.
             * 
             * @param cube_map_data - The data for which to copy.
             * @param reload - Wheter or not to reload the texture data.
             */
            inline void Set_Cube_Map_Data(const Cube_Map_Data& cube_map_data, const bool& reload = false) {
                m_cube_map_data = cube_map_data;
                if (reload)
                    m_cube_map_data.Reload();
            }
            /**
             * @brief Moves all of the data from the given cube map data. If told so will also
             *        load the texture data from the paths.
             *
             * @param cube_map_data - The data for which to move from.
             * @param reload - Wheter or not to reload the texture data.
             */
            inline void Set_Cube_Map_Data(Cube_Map_Data&& cube_map_data, const bool& reload = false) {
                m_cube_map_data = std::move(cube_map_data);
                if (reload)
                    m_cube_map_data.Reload();
            }

            /**
             * @brief Gets the cube map data.
             * 
             * @return The cube map data of the cube map.
             */
            inline auto Get_Cube_Map_Data() const {
                return m_cube_map_data;
            }

            /**
             * @brief Sets the paths of the sides of the cube map to the given sides.
             * 
             * @param file_paths - The paths to be set to.
             * @param reload - Wheter or not to reload the texture data.
             */
            inline void Set_Paths(const std::array<std::string, *enums::Misc::Cube_Sides>& file_paths, const bool& reload = false) {
                for (std::size_t i = 0; i < *enums::Misc::Cube_Sides; i++)
                {
                    m_cube_map_data.sides[i].file_path = file_paths[i];
                }
                if (reload)
                    m_cube_map_data.Reload();
            }
            /**
             * @brief Sets the paths of the sides of the cube map to the given sides.
             *
             * @param file_paths - The paths to be set to.
             * @param reload - Wheter or not to reload the texture data.
             */
            inline void Set_Paths(std::array<std::string, *enums::Misc::Cube_Sides>&& file_paths, const bool& reload = false) {
                for (std::size_t i = 0; i < *enums::Misc::Cube_Sides; i++)
                {
                    m_cube_map_data.sides[i].file_path = std::move(file_paths[i]);
                }
                if (reload)
                    m_cube_map_data.Reload();
            }

            /**
             * @brief Gets the paths of the cube map.
             * 
             * @return The paths.
             */
            inline auto Get_Paths() const {
                std::array<std::string, *enums::Misc::Cube_Sides> ret_file_paths{};
                for (std::size_t i = 0; i < *enums::Misc::Cube_Sides; i++)
                {
                    ret_file_paths[i] = m_cube_map_data.sides[i].file_path;
                }
                return ret_file_paths;
            }

            /**
             * @brief Sets the path of the given index to the given path.
             * 
             * @param index - The path of the path to set.
             * @param file_path - The path to be set to.
             * @param reload - Wheter or not to reload the texture data.
             */
            inline void Set_Path(const std::size_t& index, const std::string& file_path, const bool& reload = false) {
                m_cube_map_data.sides[index].file_path = file_path;
                if (reload)
                    m_cube_map_data.Reload(index);
            }
            /**
             * @brief Sets the path of the given index to the given path.
             *
             * @param index - The path of the path to set.
             * @param file_path - The path to be set to.
             * @param reload - Wheter or not to reload the texture data.
             */
            inline void Set_Path(const std::size_t& index, std::string&& file_path, const bool& reload = false) {
                m_cube_map_data.sides[index].file_path = std::move(file_path);
                if (reload)
                    m_cube_map_data.Reload(index);
            }

            /**
             * @brief Gets the path of the given index.
             * 
             * @param index - The index of the path to get.
             * 
             * @return The path of the given index.
             */
            inline auto Get_Path(const std::size_t& index) const {
                return m_cube_map_data.sides[index].file_path;
            }

            /**
             * @brief Copies the texture data for the side of the given index from the given data.
             * 
             * @param index - The index of the side of which data to set.
             * @param data - The data for which to copy from.
             * @param byte_count - The amount of bytes the data is made up of. If zero then will
             *                     use the square of the already stored size.
             */
            inline void Set_Data(const std::size_t& index, uint8_t*& data, const uint32_t& byte_count = 0) {
                m_cube_map_data.Copy_Data(index, data, byte_count);
            }
            /**
             * @brief Moves the texture data for the side of the given index from the given data.
             *
             * @param index - The index of the side of which data to set.
             * @param data - The data for which to move from.
             */
            inline void Set_Data(const std::size_t& index, uint8_t*&& data) {
                m_cube_map_data.sides[index].texture_data.reset(data);
            }

            /**
             * @brief Gets the data from the given index. If specified then the ownership of the
             *        data will be released.
             * 
             * @param index - The index of the side of which data to get.
             * @param take_ownership - Wheter or not to release the ownership of the data.
             * 
             * @return The texture data of the index.
             */
            inline auto Get_Data(const std::size_t& index, const bool& take_ownership = false) {
                if (take_ownership)
                    return m_cube_map_data.sides[index].texture_data.release();
                else
                    return m_cube_map_data.sides[index].texture_data.get();
            }

            /**
             * @brief Sets the color format of the given index.
             * 
             * @param index - The index of the color format to set.
             * @param color_format - The color format of which to be set to.
             */
            inline void Set_Format(const std::size_t& index, const enums::Color_Format& color_format) {
                m_cube_map_data.sides[index].color_format = color_format;
            }

            /**
             * @brief Gets the color format of the given index.
             * 
             * @param index - The index of the color format to get.
             * 
             * @return The color format of the index.
             */
            inline auto Get_Format(const std::size_t& index) const {
                return m_cube_map_data.sides[index].color_format;
            }

            /**
             * @brief Sets the color format of the data of the given index.
             * 
             * @param index - The index of the color format of the data to set.
             * @param data_color_format - The color format to set the color format of the data to.
             */
            inline void Set_Data_Format(const std::size_t& index, const enums::Data_Color_Format& data_color_format) {
                m_cube_map_data.sides[index].data_color_format = data_color_format;
            }

            /**
             * @brief Gets the color format of the data of the given index.
             * 
             * @param index - The index of the color format of the data to get.
             * 
             * @return The color format of the data of the given index.
             */
            inline auto Get_Data_Format(const std::size_t& index) const {
                return m_cube_map_data.sides[index].data_color_format;
            }

            /**
             * @brief Sets the size of the cube map sides to the given size.
             * 
             * @param size - The size to set the size of the cube map sides to.
             */
            inline void Set_Size(const std::int32_t size) {
                m_cube_map_data.size = size;
            }

            /**
             * @brief Gets the size of the sides of the cube map.
             * 
             * @return The size of the sides of the cube map.
             */
            inline auto Get_Size() const {
                return m_cube_map_data.size;
            }

            /**
             * @brief Sets the filtering mode for the given filtering size.
             *
             * @param filter_size - The size of filtering for which to set the mode of.
             * @param filter_mode - The mode of filtreing for which to use for the size.
             */
            void Set_Filter(const enums::Filter_Size& filter_size, const enums::Filter_Mode& filter_mode) override;

            /**
             * @brief Gets the filter mode of the given filter size.
             * 
             * @param filter_size - The filter size of the filter mode to get.
             * 
             * @return The filter mode of the given filter size.
             */
            inline auto Get_Filter(const enums::Filter_Size& filter_size) const {
                switch (filter_size)
                {
                case enums::Filter_Size::Magnify:
                    return m_cube_map_data.filter_mag;
                case enums::Filter_Size::Minify:
                    return m_cube_map_data.filter_min;
                }
            }

            /**
             * @brief Set wrapping for the given side.
             *
             * @param wrap_side - The side of which to set wrapping for.
             * @param wrap_mode - The wrapping mode to set for the given side.
             */
            void Set_Wrapping(const enums::Wrap_Sides& wrap_side, const enums::Wrap_Mode& wrap_mode) override;

            /**
             * @brief Gets the wrapping mode of the given wrapping side.
             * 
             * @param wrap_side - The wrapping side of the wrapping mode to get.
             * 
             * @return The wrapping mode of the given wrapping side.
             */
            inline auto Get_Wrapping(const enums::Wrap_Sides& wrap_side) const {
                switch (wrap_side)
                {
                case enums::Wrap_Sides::S:
                    return m_cube_map_data.wrap_s;
                case enums::Wrap_Sides::T:
                    return m_cube_map_data.wrap_t;
                case enums::Wrap_Sides::R:
                    return m_cube_map_data.wrap_r;
                }
            }

        private:

            // The info pertaining to this Texture
            Cube_Map_Data m_cube_map_data;

        }; // Cube_Map

    } // render

} // tilia

#endif // TILIA_CUBE_MAP_HPP