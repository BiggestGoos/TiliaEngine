// Vendor
#include "vendor/glad/KHR_Debug_openGL_3_3/include/glad/glad.h"

// Standard
#include <iostream>

// Tilia
#include "Error_Handling.hpp"
#include "Core/Values/Directories.hpp"
#include TILIA_LOGGING_INCLUDE

static constexpr const char* Get_Error_String(std::uint32_t error_code) {
    switch (error_code)
    {
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW";
    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    default:
        return "Unknown Error";
    }
}

void tilia::utils::Handle_OpenGL_Error(const char* file, std::size_t line, const char* function)
{
    GLenum error{ glGetError() };
    if (error == GL_NO_ERROR)
        return;

    Exception_Data e_d{ file, line, "There have been one or more openGL errors:\nFunction: ", 
        function, "\n\n" };

    std::size_t error_index{ 1 };
    do
    {
        e_d.Append_Message("OpenGL error #", error_index, 
            "\n>>> Code: ", error,
            "\n>>> Name: ", Get_Error_String(error), "\n\n");
        ++error_index;
    } 
    while (error = glGetError());

    Tilia_Exception t_e{ e_d };

    throw t_e;
}

void tilia::utils::Handle_OpenGL_Error(const char* file, std::size_t line, const char* function,
    Exception_Data message)
{
    GLenum error{ glGetError() };
    if (error == GL_NO_ERROR)
        return;

    message.Set_Location(file, line);

    message.Append_Message("There have been one or more openGL errors:\nFunction: ",
        function, "\n\n");

    std::size_t error_index{ 1 };
    do
    {
        message.Append_Message("OpenGL error #", error_index,
            "\n>>> Code: ", error,
            "\n>>> Name: ", Get_Error_String(error), "\n\n");
        ++error_index;
    } 
    while (error = glGetError());

    Tilia_Exception t_e{ message };

    throw t_e;
}

void tilia::utils::Clear_OpenGL_Error()
{
    while (glGetError() != GL_NO_ERROR);
}

#if TILIA_UNIT_TESTS == 1

// Vendor
#include "vendor/Catch2/Catch2.hpp"

void tilia::utils::Error_Handling::Test()
{

    // Check that clearing the openGL errors work

    glEnable(GL_FALSE);
    glEnable(GL_TEXTURE0);
    glEnable(GL_2_BYTES);

    Clear_OpenGL_Error();

    REQUIRE(glGetError() == GL_NO_ERROR);



}

#endif // TILIA_UNIT_TESTS == 1