// Standard
#include <iostream>

// Tilia
#include "Logging.hpp"

void tilia::log::Logger::Output(const std::string& data)
{
}

void tilia::log::Logger::Output(const utils::Exception_Data& data)
{
    std::lock_guard lock{ m_mutex };
    static auto potential_message{ [](const auto& message)
    {
        const auto message_length{ message.size() };
        if (message_length > 0)
            return message.c_str();
        return "Message is empty";
    } };
    auto location{ data.Get_Location() };
    (*m_output) << "Tilia Exception Data:\n"
        << "File: " << location.first << " : Line: " << location.second
        << "\nMessage:\n" << potential_message(data.Get_Message()) << '\n';
}

void tilia::log::Logger::Output(const utils::Tilia_Exception& data)
{
    std::lock_guard lock{ m_mutex };
    (*m_output) << "Tilia Exception:\n\n" << data.what();
}

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

tilia::log::Logger::Logger()
    : m_output{ &std::cout } { }
