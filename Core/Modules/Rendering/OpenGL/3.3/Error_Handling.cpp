/*****************************************************************//**
 * @file   Error_Handling.cpp
 * @brief  A source file for @include "headers/Error_Handling.h", that defines all non inline 
 *         functions declared in @include "headers/Error_Handling.h".
 * 
 * Dependencies:
 * @include "dependencies/glad/include/glad/glad.h"
 * 
 * Standard:
 * @include <iostream>
 * 
 * Headers:
 * @include "headers/Error_Handling.h"
 * @include "headers/Logging.h"
 * 
 * @author Gustav Fagerlind
 * @date   15/05/2022
 *********************************************************************/

// Dependencies
#include "vendor/glad/include/glad/glad.h"

// Standard
#include <iostream>

// Headers
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.hpp"
#include "Core/Modules/Console/Logging.hpp"
#include "Core/Modules/Exceptions/Tilia_Exception.hpp"

/**
 * Checks what error string pertains to error_code. If there is no
 * error string for error_code then it returns "Something went wrong".
 */
static constexpr const char* Get_Error_String(const uint32_t& error_code) {
    switch (error_code)
    {
    case 0x500:
        return "GL_INVALID_ENUM";
    case 0x501:
        return "GL_INVALID_VALUE";
    case 0x502:
        return "GL_INVALID_OPERATION";
    case 0x503:
        return "GL_STACK_OVERFLOW";
    case 0x504:
        return "GL_STACK_UNDERFLOW";
    case 0x505:
        return "GL_OUT_OF_MEMORY";
    case 0x506:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case 0x507:
        return "GL_CONTEXT_LOST";
    case 0x8031:
        return "GL_TABLE_TOO_LARGE1";
    default:
        return "Unknown Error";
    }
}

void tilia::utils::Handle_GL_Error(const char* message, const size_t& line, const char* file, const char* function)
{
    // Checks errors
    while (GLenum error = glGetError()) {
        utils::Tilia_Exception e{ line, file };
        auto x{ e.Add_Message("OpenGL [ Error was thrown ]"
            "\n>>> Code: %v"
            "\n>>> Name: %v"
            "\n>>> Func: %v")
        (error)(Get_Error_String(error))(function) };
        if (message != "")
            x("\n>>> Message: ")(message);
        throw e;
    }
}

bool tilia::utils::GL_Check_Error()
{
    // Checks errors
    while (glGetError()) {
        return false;
    }
    return true;
}

/**
* Clears the openGL errors.
*/
void tilia::utils::GL_Clear_Error()
{
    while (glGetError() != GL_NO_ERROR);
}
