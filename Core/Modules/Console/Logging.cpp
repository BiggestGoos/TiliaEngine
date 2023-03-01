// Tilia
#include "Logging.hpp"

void tilia::log::Logger::OpenGL_Error_Callback(std::uint32_t source, std::uint32_t type, 
    std::uint32_t severity, std::uint32_t id, std::int32_t length, const char* message, 
    const void* user_param)
{
    Logger& logger{ Logger::Instance() };
    (*logger.m_output) << "OpenGL error"
    << "\nSource: " << source
    << "\nType: " << type
    << "\nId: " << id
    << "\nSeverity: " << severity
    << "\nMessage: " << message 
    << "\nUser Param: " << user_param << '\n';

}

void tilia::log::Logger::GLFW_Error_Callback(std::int32_t error_code, const char* description)
{

    Logger& logger{ Logger::Instance() };
    (*logger.m_output) << "GLFW error"
        << "\nError Code: " << error_code
        << "\nDescription: " << description << '\n';

}
