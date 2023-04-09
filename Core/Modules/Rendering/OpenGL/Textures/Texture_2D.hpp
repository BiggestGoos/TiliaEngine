/**************************************************************************************************
 * @file   Texture_2D.hpp
 * 
 * @brief  
 * 
 * @author Gustav Fagerlind
 * @date   09/04/2023
 *************************************************************************************************/

#ifndef TILIA_TEXTURE_2D_HPP
#define TILIA_TEXTURE_2D_HPP

// Tilia
#include "Core/Values/Directories.hpp"
#include TILIA_CONSTANTS_INCLUDE
#include TILIA_TEXTURE_INCLUDE
#include TILIA_IMAGE_INCLUDE

namespace tilia
{

	namespace texturing
	{

		class Texture_2D : public Texture
		{
		public:

			void Init(Image image = {});

			void Terminate();



		private:

			Image m_texture_image{};

		}; // Texture_2D

	} // texturing

} // tilia

#endif // TILIA_TEXTURE_2D_HPP