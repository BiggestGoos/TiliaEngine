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
#include "dependencies/glad/include/glad/glad.h"

// Standard
#include <iostream>

// Headers
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.h"
#include "Core/Modules/Console/Logging.h"
#include "Core/Modules/Exceptions/Tilia_Exception.h"

/**
 * Checks what error string pertains to error_code. If there is no
 * error string for error_code then it returns "Something went wrong".
 */
const char* Get_Error_String(const uint32_t& error_code) {
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

void Handle_GL_Error(const char* message, const size_t& line, const char* file, const char* function)
{
    // Checks errors
    while (GLenum error = glGetError()) {
        std::stringstream ss{};
        ss << "OpenGL [ Error was thrown ]" <<
              "\nCode: 0x" << error <<
              "\nName: " << Get_Error_String(error) <<
              "\nFunc: " << function;
        if (message != "")
              ss << "\nMessage: " << message;
        throw tilia::utils::Tilia_Exception{ "", line, file };
    }
}

/**
 * Clears the openGL errors.
 */
void GL_Clear_Error()
{
    while (glGetError() != GL_NO_ERROR);
}

/**
 * Checks if there is an openGL error. If so then it prints errors and 
 * things pertaining to this error using Log and Log_Indent and then 
 * returns false. Else it just returns true.
 */
bool GL_Log_Call(const char* function, const char* file, int line)
{
    // Checks errors
    while (GLenum error = glGetError()) {
        // Prints errors and stuff
        tilia::log::Log(tilia::log::Type::ERROR, "OpenGL", "Error was thrown");
        tilia::log::Log_Indent("Code", "0x%x", error);
        tilia::log::Log_Indent("Name", "%s", Get_Error_String(error));
        tilia::log::Log_Indent("File", "%s", file);
        tilia::log::Log_Indent("Line", "%d", line);            
        tilia::log::Log_Indent("Func", "%s", function);
        return false;
    }
    return true;
}