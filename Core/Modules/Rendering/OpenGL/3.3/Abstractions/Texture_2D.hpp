/*****************************************************************//**
 * @file   Texture_2D.h
 * @brief  Declares and defines a class called @Texture_2D deriving from @Texture in @include "headers/Texture.h", which works as an abstraction for an openGL 2d texture 
 *		   as well as a struct called @Texture_2D_Def that holds some information about the class.
 * 
 * @define TILIA_TEXTURE_H
 * 
 * Standard:
 * @include <string>
 * @include <memory>
 * 
 * Headers:
 * @include "headers/Texture.h"
 * @include "headers/Enums.h"
 * 
 * @author Gustav Fagerlind
 * @date   15/05/2022
 *********************************************************************/

#ifndef TILIA_TEXTURE_2D_H
#define TILIA_TEXTURE_2D_H

// Standard
#include <string>
#include <memory>

// Headers
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Texture.hpp"
#include "Core/Values/OpenGL/3.3/Enums.hpp"

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
		 * @param filter_min        - The filtering mode when the texture has been zoomed in  - Default: Filter_Mode::Point
		 * @param filter_mag        - The filtering mode when the texture has been zoomed out - Default: Filter_Mode::Point
		 * @param wrap_s	        - The wrapping mode for the x-axis						  - Default: Wrap_Mode::Repeat
		 * @param wrap_t	        - The wrapping mode for the y-axis						  - Default: Wrap_Mode::Repeat
		 */
		struct Texture_2D_Def
		{
			std::string 					   file_path{};
			std::unique_ptr<uint8_t[]>		   texture_data{};
			int32_t							   width{};
			int32_t							   height{};
			enums::Color_Format	   color_format{ enums::Color_Format::RGBA8 };
			enums::Data_Color_Format				   load_color_format{ enums::Data_Color_Format::None };
			enums::Filter_Mode				   filter_min{ enums::Filter_Mode::Point };
			enums::Filter_Mode				   filter_mag{ enums::Filter_Mode::Point };
			enums::Wrap_Mode				   wrap_s{ enums::Wrap_Mode::Repeat };
			enums::Wrap_Mode				   wrap_t{ enums::Wrap_Mode::Repeat };
			/**
			 * @brief Assignment operator. Shallow copies everything except for texture_data which will get reset to nullptr
			 */
			const Texture_2D_Def& operator=(const Texture_2D_Def& other) {
				if (&other == this)
					return *this;
				
				this->file_path = other.file_path;
				this->texture_data.reset();
				this->width = other.width;
				this->height = other.height;
				this->color_format = other.color_format;
				this->load_color_format = other.load_color_format;
				this->filter_min = other.filter_min;
				this->filter_mag = other.filter_mag;
				this->wrap_s = other.wrap_s;
				this->wrap_t = other.wrap_t;

				return *this;
			}
		};

		/**
		 * @brief A class deriving from Texture class which works as an abstraction for an openGL texture. Also contains a Texture_Def struct
		 */
		class Texture_2D : public Texture {
		public:

			/**
			 * @brief The default constructor which generates an openGL texture and sets the texture id
			 */
			Texture_2D();

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
			void Set_Texture(const Texture_2D_Def& texture_def);

			/**
			 * @brief Sets the Texture_Def of this Texture to have the path given.
			 * Then loads in the texture data from this file path.
			 *
			 * @param texture_path - The path of the texture to load
			 *
			 * @return A return code. If 0 success.
			 *
			 * @exception Code: 2 - Failed to load data
			 */
			void Set_Texture(const std::string& texture_path);

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
			inline const Texture_2D_Def& Get_Texture_Def() const { return m_texture_def; }

			/**
			 * @brief Returns the width of this Texture's Texture_Def
			 *
			 * @return m_texture_def.width - The width of this Texture's Texture_Def
			 */
			inline const int32_t& Get_Width() const { return m_texture_def.width; }
			/**
			 * @brief Returns the height of this Texture's Texture_Def
			 *
			 * @return m_texture_def.height - The height of this Texture's Texture_Def
			 */
			inline const int32_t& Get_Height() const { return m_texture_def.height; }

			/**
			 * @brief Sets the filtering mode for the given filtering size
			 *
			 * @param filter_size - The size of filtering for which to set the mode of
			 * @param filter_mode - The mode of filtreing for which to use for the size
			 */
			void Set_Filter(const enums::Filter_Size& filter_size, const enums::Filter_Mode& filter_mode) override;

			/**
			 * @brief Set wrapping for the given side
			 *
			 * @param wrap_side - The side of which to set wrapping for
			 * @param wrap_mode - The wrapping mode to set for the given side
			 */
			void Set_Wrapping(const enums::Wrap_Sides& wrap_side, const enums::Wrap_Mode& wrap_mode) override;

		private:

			Texture_2D_Def m_texture_def; // The info pertaining to this Texture
			
			/**
			 * @brief Prints information about the shader indented. Prints the dimensions, color format,
			 * filtering modes and wrapping modes.
			 */
			void Print_Information() const;

		};

	}

}

#endif