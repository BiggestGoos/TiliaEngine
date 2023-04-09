// Vendor
#include "vendor/glad/KHR_Debug_openGL_3_3/include/glad/glad.h"

// Tilia
#include "Texture_2D.hpp"
#include TILIA_OPENGL_3_3_ERROR_HANDLING_INCLUDE

void tilia::texturing::Texture_2D::Init(Image image)
{
	GL_CALL(glGenTextures(1, &m_ID));

	

}

void tilia::texturing::Texture_2D::Terminate()
{
}
