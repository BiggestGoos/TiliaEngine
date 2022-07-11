/*****************************************************************//**
 * @file   Cube_Map.h
 * @brief  Declares and defines a class called @Cube_Map deriving from @Texture in @include "headers/Texture.h", 
 *         which works as an abstraction for an openGL cube map texture as well as a struct called @Cube_Map_Def 
 *         that holds some information about the class.
 * 
 * @define TILIA_CUBE_MAP_H
 * 
 * Standard:
 * @include <array>
 * @include <memory>
 * 
 * Headers:
 * @include "headers/Texture.h"
 * 
 * @author Gustav Fagerlind
 * @date   04/06/2022
 *********************************************************************/

// Standard
#include <array>
#include <memory>

// Headers
#include "headers/Rendering/OpenGL/3.3/Abstractions/Texture.h"

#ifndef TILIA_CUBE_MAP_H
#define TILIA_CUBE_MAP_H

namespace tilia {

	namespace render {

		/**
		 * @brief A struct that holds information for the Texture_2D class
		 *
		 * @param file_path	        - The file path of the texture
		 * @param texture_data      - The texel data of the texture
		 * @param width             - The width of the texture
		 * @param height		    - The height of the texture
		 * @param color_format      - The color format of the texture						  - Default: Color_Format::RGBA8
		 * @param load_color_format - The color format loaded in							  - Default: Color_Format::None
		 * @param filter_min        - The filtering mode when the textures has been zoomed in  - Default: Filter_Mode::Point
		 * @param filter_mag        - The filtering mode when the textures has been zoomed out - Default: Filter_Mode::Point
		 * @param wrap_s	        - The wrapping mode for x-axis		      - Default: Wrap_Mode::Repeat
		 * @param wrap_t	        - The wrapping mode for y-axis				  - Default: Wrap_Mode::Repeat
		 * @param wrap_r	        - The wrapping mode for z-axis				  - Default: Wrap_Mode::Repeat
		 */
		struct Cube_Map_Def
		{
			struct Cube_Side {
				std::string file_path{};
				std::unique_ptr<uint8_t[]> texture_data{};
				enums::Color_Format	color_format{ enums::Color_Format::RGBA8 };
				enums::Color_Format	load_color_format{ enums::Color_Format::None };
			};
			std::array<Cube_Side, *enums::Misc::Cube_Sides> sides;
			int32_t							   size{};
			enums::Filter_Mode				   filter_min{ enums::Filter_Mode::Point };
			enums::Filter_Mode				   filter_mag{ enums::Filter_Mode::Point };
			enums::Wrap_Mode				   wrap_s{ enums::Wrap_Mode::Repeat };
			enums::Wrap_Mode				   wrap_t{ enums::Wrap_Mode::Repeat };
			enums::Wrap_Mode				   wrap_r{ enums::Wrap_Mode::Repeat };
			/**
			 * @brief Assignment operator. Shallow copies everything except for the data of all the sides which will get reset to nullptr
			 */
			const Cube_Map_Def& operator=(const Cube_Map_Def& other) {
				if (&other == this)
					return *this;

				size_t side_count{ this->sides.size() };
				for (size_t i = 0; i < side_count; i++)
				{
					this->sides[i].file_path = other.sides[i].file_path;
					this->sides[i].texture_data.reset();
					this->sides[i].color_format = other.sides[i].color_format;
					this->sides[i].load_color_format = other.sides[i].load_color_format;
				}

				this->size = other.size;
				this->filter_min = other.filter_min;
				this->filter_mag = other.filter_mag;
				this->wrap_s = other.wrap_s;
				this->wrap_t = other.wrap_t;
				this->wrap_r = other.wrap_r;

				return *this;
			}
		};

		/**
		 * @brief A class deriving from Texture class which works as an abstraction for an openGL texture. Also contains a Texture_Def struct
		 */
		class Cube_Map : public Texture {
		public:

			/**
			 * @brief The default constructor which generates an openGL texture and sets the texture id
			 */
			Cube_Map();

			/**
			 * @brief Sets the Texture_Def of this Texture to the param Texture_Def.
			 * If the texture_data of the given texture_def is null then the texture data
			 * is set to the loaded data from the given file_path
			 *
			 * @param texture_def - The Texture_Def for which to set this Texture's Texture_Def to
			 *
			 * @return A return code. If 0 success.
			 *
			 * @exception Code: 1 - Failed to copy data
			 * @exception Code: 2 - Failed to load data
			 */
			int32_t Set_Texture(const Cube_Map_Def& texture_def);

			/**
			 * @brief Sets the Cube_Map_Def of this Texture to have the paths given.
			 * Then loads in the texture data from the file paths.
			 *
			 * @param texture_paths - The paths of the textures to load
			 *
			 * @return A return code. If 0 success.
			 *
			 * @exception Code: 2 - Failed to load data
			 */
			int32_t Set_Texture(const std::array<std::string, 6>& texture_paths);

			/**
			 * @brief Generates all mipmap levels for the texture
			 *
			 * @exception Texture is not loaded
			 * @exception There is no texture_data
			 */
			void Generate_Mipmaps() override;

			/**
			 * @brief Returns the Texture_Def of this Texture
			 *
			 * @return m_texture_def - The Texture_Def of this Texture
			 */
			inline const Cube_Map_Def& Get_Texture_Def() const { return m_texture_def; }

			/**
			 * @brief Returns the size of this Texture's Texture_Def
			 *
			 * @return m_texture_def.size - The size of this Texture's Texture_Def
			 */
			inline const int32_t& Get_Size() const { return m_texture_def.size; }

			/**
			 * @brief Sets the filtering mode for the given filtering size
			 *
			 * @param filter_size - The size of filtering for which to set the mode of
			 * @param filter_mode - The mode of filtreing for which to use for the size
			 */
			void Set_Parameter(const enums::Filter_Size& filter_size, const enums::Filter_Mode& filter_mode) override;

			/**
			 * @brief Set wrapping for the given side
			 *
			 * @param wrap_side - The side of which to set wrapping for
			 * @param wrap_mode - The wrapping mode to set for the given side
			 */
			void Set_Parameter(const enums::Wrap_Sides& wrap_side, const enums::Wrap_Mode& wrap_mode) override;

		private:

			Cube_Map_Def m_texture_def; // The info pertaining to this Texture

			/**
			 * @brief Prints information about the shader indented. Prints the dimensions, color format,
			 * filtering modes and wrapping modes.
			 */
			void Print_Information() const;

		};

	}

}

#endif