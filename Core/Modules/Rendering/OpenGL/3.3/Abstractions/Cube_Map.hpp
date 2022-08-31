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

        // Can be used to set and or get specific data from the Cube_Map class.
        enum class Cube_Map_Var {
            Cube_Map_Data,
            Paths,
            Path,
            Data,
            Format,
            Data_Format,
            Size,
            Filter,
            Wrapping,
        };

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
            const Cube_Map_Data& operator=(const Cube_Map_Data& other);

            Cube_Map_Data() = default;

            /**
             * @brief Simple copy-constructor which calls copy-assignment.
             * 
             * @param other - The Cube_Map_Data for which to copy from.
             */
            Cube_Map_Data(const Cube_Map_Data& other);

            /**
             * @brief Reloads the texture data of the side with the given index.
             * 
             * @param index - The index of the side to reload the data of.
             */
            void Reload(const std::size_t& index);

            /**
             * @brief Reloads the texture data of all of the sides.
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

            //~Cube_Map();

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
             * @brief Generates all mipmap levels for the cube map.
             *
             * @exception Guarantee: Basic
             * @exception Reasons:
             * @exception Any of the side of the cube map is missing texture data.
             * @exception The cube map is not cube complete.
             */
            void Generate_Mipmaps() override;

            template<Cube_Map_Var variable>
            inline auto Set();

            template<Cube_Map_Var variable>
            inline auto Get();

            /**
             * @brief w
             * 
             * @return 
             */
            template<>
            inline auto Set<Cube_Map_Var::Cube_Map_Data>() {
                return [this](const Cube_Map_Data& cube_map_data, const bool& reload = false) 
                { 
                    m_cube_map_data = cube_map_data; 
                    if (reload)
                    {
                        m_cube_map_data.Reload();
                        Reload();
                    }
                };
            }

            template<>
            inline auto Get<Cube_Map_Var::Cube_Map_Data>() {
                return m_cube_map_data;
            }

            template<>
            inline auto Set<Cube_Map_Var::Paths>() {
                return [this](const std::array<std::string, *enums::Misc::Cube_Sides>& file_paths, const bool& reload = false)
                {
                    for (std::size_t i = 0; i < *enums::Misc::Cube_Sides; i++)
                    {
                        m_cube_map_data.sides[i].file_path = file_paths[i];
                    }
                    if (reload)
                    {
                        m_cube_map_data.Reload();
                        Reload();
                    }
                };
            }

            template<>
            inline auto Get<Cube_Map_Var::Paths>() {
                std::array<std::string, *enums::Misc::Cube_Sides> ret_file_paths{};
                for (std::size_t i = 0; i < *enums::Misc::Cube_Sides; i++)
                {
                    ret_file_paths[i] = m_cube_map_data.sides[i].file_path;
                }
                return ret_file_paths;
            }

            template<>
            inline auto Set<Cube_Map_Var::Path>() {
                return [this](const std::size_t& index, const std::string& file_path, const bool& reload = false)
                {
                    m_cube_map_data.sides[index].file_path = file_path;
                    if (reload)
                    {
                        m_cube_map_data.Reload(index);
                    }
                    return [this]() {
                        Reload();
                    };
                };
            }

            template<>
            inline auto Get<Cube_Map_Var::Path>() {
                return [this](const std::size_t& index)
                {
                    return m_cube_map_data.sides[index].file_path;
                };
            }

            template<>
            inline auto Set<Cube_Map_Var::Data>() {
                return [this](const std::size_t& index, uint8_t* data, const bool& release_ownership = false, 
                    const uint32_t& byte_count = 0)
                {
                    if (release_ownership)
                        m_cube_map_data.sides[index].texture_data.reset(data);
                    else
                        m_cube_map_data.Copy_Data(index, data, byte_count);
                    return [this]() {
                        Reload();
                    };
                };
            }

            template<>
            inline auto Get<Cube_Map_Var::Data>() {
                return [this](const std::size_t& index, const bool& take_ownership = false)
                {
                    if (take_ownership)
                        return m_cube_map_data.sides[index].texture_data.release();
                    else
                        return m_cube_map_data.sides[index].texture_data.get();
                };
            }

            template<>
            inline auto Set<Cube_Map_Var::Format>() {
                return [this](const std::size_t& index, const enums::Color_Format& color_format, const bool& reload = false)
                {
                    m_cube_map_data.sides[index].color_format = color_format;
                    if (reload)
                        Reload();
                };
            }

            template<>
            inline auto Get<Cube_Map_Var::Format>() {
                return [this](const std::size_t& index)
                {
                    return m_cube_map_data.sides[index].color_format;
                };
            }

            template<>
            inline auto Set<Cube_Map_Var::Data_Format>() {
                return [this](const std::size_t& index, const enums::Data_Color_Format& data_color_format, const bool& reload = false)
                {
                    m_cube_map_data.sides[index].data_color_format = data_color_format;
                    if (reload)
                        Reload();
                };
            }

            template<>
            inline auto Get<Cube_Map_Var::Data_Format>() {
                return [this](const std::size_t& index)
                {
                    return m_cube_map_data.sides[index].data_color_format;
                };
            }

            template<>
            inline auto Set<Cube_Map_Var::Size>() {
                return [this](const std::int32_t size, const bool& reload = false)
                {
                    m_cube_map_data.size = size;
                    if (reload)
                        Reload();
                };
            }

            template<>
            inline auto Get<Cube_Map_Var::Size>() {
                return [this]()
                {
                    return m_cube_map_data.size;
                };
            }

            template<>
            inline auto Set<Cube_Map_Var::Filter>() {
                return [this](const enums::Filter_Size& filter_size, const enums::Filter_Mode& filter_mode, const bool& reload = false)
                {
                    Set(filter_size, filter_mode);
                    if (reload)
                        Reload();
                };
            }

            template<>
            inline auto Get<Cube_Map_Var::Filter>() {
                return [this](const enums::Filter_Size& filter_size)
                {
                    switch (filter_size)
                    {
                    case enums::Filter_Size::Magnify:
                        return m_cube_map_data.filter_mag;
                    case enums::Filter_Size::Minify:
                        return m_cube_map_data.filter_min;
                    }
                };
            }

            template<>
            inline auto Set<Cube_Map_Var::Wrapping>() {
                return [this](const enums::Wrap_Sides& wrap_side, const enums::Wrap_Mode& wrap_mode, const bool& reload = false)
                {
                    Set(wrap_side, wrap_mode);
                    if (reload)
                        Reload();
                };
            }

            template<>
            inline auto Get<Cube_Map_Var::Wrapping>() {
                return [this](const enums::Wrap_Sides& wrap_side)
                {
                    switch (wrap_side)
                    {
                    case enums::Wrap_Sides::S:
                        return m_cube_map_data.wrap_s;
                    case enums::Wrap_Sides::T:
                        return m_cube_map_data.wrap_t;
                    case enums::Wrap_Sides::R:
                        return m_cube_map_data.wrap_r;
                    }
                };
            }

        private:

            // The info pertaining to this Texture
            Cube_Map_Data m_cube_map_data;

            /**
             * @brief Sets the filtering mode for the given filtering size.
             *
             * @param filter_size - The size of filtering for which to set the mode of.
             * @param filter_mode - The mode of filtreing for which to use for the size.
             */
            void Set(const enums::Filter_Size& filter_size, const enums::Filter_Mode& filter_mode) override;

            /**
             * @brief Set wrapping for the given side.
             *
             * @param wrap_side - The side of which to set wrapping for.
             * @param wrap_mode - The wrapping mode to set for the given side.
             */
            void Set(const enums::Wrap_Sides& wrap_side, const enums::Wrap_Mode& wrap_mode) override;


        }; // Cube_Map

    } // render

} // tilia

#endif // TILIA_CUBE_MAP_HPP