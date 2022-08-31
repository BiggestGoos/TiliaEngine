/*****************************************************************//**
 * @file   Texture.h
 * @brief  Contains the @Texture class which works as a base class for 
 *		   various different classes abstracting openGL texture objects
 * 
 * @define TILIA_TEXTURE_H
 * 
 * Standard:
 * @include <stdint.h>
 * @include <string>
 * @include <unordered_map>
 * 
 * Headers:
 * @include "headers/Enums.h"
 * 
 * @author Gustav Fagerlind
 * @date   02/06/2022
 *********************************************************************/

#ifndef TILIA_TEXTURE_H
#define TILIA_TEXTURE_H

// Standard
#include <stdint.h>
#include <string>
#include <unordered_map>

// Headers
#include "Core/Values/OpenGL/3.3/Enums.hpp"

namespace tilia {

	namespace render {

		// Base class for texture classes
		class Texture {
		public:

			/**
			 * @brief The default destructor which deletes the openGL texture of this id
			 */
			~Texture();

			 /**
			 * @brief Generates all mipmap levels for the texture
			 *
			 * @exception Texture is not loaded
			 * @exception There is no texture_data
			 */
			virtual void Generate_Mipmaps() = 0;

			/**
			 * @brief Gets the type of openGL texture this is
			 * 
			 * @return m_texture_type - The type of texture
			 */
			inline const enums::Texture_Type& Get_Texture_Type() { return m_texture_type; }

			/**
			 * @brief Binds the texture to the given texture slot. If slot is outside of 
			 * range, prints errors.
			 * 
			 * @param slot - The texture slot to bind this texture to
			 */
			void Bind(const uint32_t& slot = 0) const;

			/**
			 * @brief Unbinds the bound texture. If save_id param is true, then saves 
			 * the previously bound texture's id.
			 * 
			 * @param save_id - If true stores previous texture id
			 */
			void Unbind(const bool& save_id = false) const;

			/**
			 * @brief Unbinds the bound texture of param texture_type. Static version
			 * of Unbind.
			 * 
			 * @param texture_type - The type of texture to unbind.
			 */
			static void Unbind(const enums::Texture_Type& texture_type, const bool& save_id = false);

			/**
			 * @brief Binds the stored previously bound texture id.
			 */
			void Rebind() const;

			/**
			 * @brief Binds the stored previously bound texture id. Static version
			 * of Rebind.
			 * 
			 * @param texture_type - The type of texture to rebind.
			 */
			static void Rebind(const enums::Texture_Type& texture_type);

			/**
			 * @brief Gets the id of the openGL texture
			 * 
			 * @return m_ID - The openGL texture id
			 */
			inline const uint32_t& Get_ID() { return m_ID; }

		protected:

			uint32_t m_ID{}; // The id of the openGL texture

			enums::Texture_Type m_texture_type{ }; // The type of openGL texture

			/**
			 * @brief Just generates an openGL texture of m_texture_type and sets m_ID
			*/
			void Generate_Texture();

		private:

			static std::unordered_map<enums::Texture_Type, uint32_t> s_bound_ID; // The stored perviously bound ids

			static std::unordered_map<enums::Texture_Type, uint32_t> s_previous_ID; // The previously bound ids

			/**
			 * @brief Sets the filtering mode for the given filtering size
			 *
			 * @param filter_size - The size of filtering for which to set the mode of
			 * @param filter_mode - The mode of filtreing for which to use for the size
			 */
			virtual void Set(const enums::Filter_Size& filter_size, const enums::Filter_Mode& filter_mode) = 0;

			/**
			 * @brief Set wrapping for the given side
			 *
			 * @param wrap_side - The side of which to set wrapping for
			 * @param wrap_mode - The wrapping mode to set for the given side
			 */
			virtual void Set(const enums::Wrap_Sides& wrap_side, const enums::Wrap_Mode& wrap_mode) = 0;

			/**
			 * @brief Gets the type of the texture as a string to be used for printing information.
			 *
			 * @return The type of Texture as a string
			 */
			std::string Get_Type_String() const;

		};

	}

}

#endif