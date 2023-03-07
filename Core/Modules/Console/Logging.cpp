// Standard
#include <iostream>
#include <algorithm>

// Tilia
#include "Logging.hpp"
#include "Core/Values/Directories.hpp"
#include TILIA_EXCEPTION_HANDLER_INCLUDE

static bool shares_filters(const std::vector<std::string>& lhs, 
    const std::vector<std::string>& rhs)
{
    if (lhs == rhs)
        return true;
    for (auto i{ lhs.cbegin() }; i < lhs.cend(); ++i)
    {
        for (auto k{ rhs.cbegin() }; k < rhs.cend(); ++k)
        {
            if (i == k)
                return true;
        }
    }
    return false;
}

void tilia::log::Logger::Output(const std::string& data, 
    std::vector<std::string> filters)
{
    std::lock_guard lock{ m_mutex };
    if (m_outputs.size() == 0)
        throw utils::Tilia_Exception{ { TILIA_LOCATION, "Logger has no viable output\n" } };

    auto used_filters{ (filters.size() == 0) ? m_filters : filters };
    for (auto& [output, output_filters] : m_outputs)
    {
        if (shares_filters(output_filters, used_filters) || used_filters.size() == 0)
        {
            (*output) << data;
        }
    }
}

void tilia::log::Logger::Output(const utils::Exception_Data& data)
{
    std::stringstream output{};
    static auto potential_message{ [](const auto& message)
    {
        const auto message_length{ message.size() };
        if (message_length > 0)
            return message.c_str();
        return "Message is empty";
    } };
    auto location{ data.Get_Location() };
    output << "Tilia Exception Data:\n"
        << "File: " << location.first << " : Line: " << location.second
        << "\nMessage:\n" << potential_message(data.Get_Message()) << '\n';
    Output(std::move(output.str()));
}

void tilia::log::Logger::Output(const utils::Tilia_Exception& data)
{
    std::stringstream output{};
    output << "Tilia Exception:\n\n" << data.what();
    Output(std::move(output.str()));
}

void tilia::log::Logger::Remove_Output(std::ostream* const output)
{
    std::lock_guard lock{ m_mutex };
    auto i{ m_outputs.cbegin() };
    for (; i < m_outputs.cend(); ++i)
    {
        if (output == i->first)
        {
            break;
        }
    }
    if (i != m_outputs.cbegin())
        m_outputs.erase(i);
}

void tilia::log::Logger::Set_Output_Filters(std::ostream* output, 
    std::vector<std::string> filters)
{
    std::lock_guard lock{ m_mutex };
    for (auto i{ m_outputs.begin() }; i < m_outputs.end(); ++i)
    {
        if (output == i->first)
        {
            i->second = std::move(filters);
        }
    }
}

/**
 * @brief Important note is that unless GL_DEBUG_OUTPUT_SYNCHRONOUS is enabled the openGL callback 
 * may be called from threads other than the main one.
 */
void tilia::log::Logger::OpenGL_Error_Callback(std::uint32_t source, std::uint32_t type,
    std::uint32_t id, std::uint32_t severity, std::int32_t length, const char* message, 
    const void* user_param)
{
    Logger& logger{ Logger::Instance() };
    utils::Exception_Handler& handler{ utils::Exception_Handler::Instance() };
    if (logger.m_outputs.size() == 0)
        return handler.Throw(utils::Tilia_Exception{ { TILIA_LOCATION, 
            "Logger has no viable output\n" } });
    std::stringstream output{};
    output << "OpenGL error"
        << "\nSource: " << source
        << "\nType: " << type
        << "\nId: " << id
        << "\nSeverity: " << severity
        << "\nMessage: " << message 
        << "\nUser Param: " << user_param << '\n';
    // To avoid data race
    decltype(logger.m_openGL_filters) temp_filters{};
    { // We do this to avoid a deadlock
        std::lock_guard lock{ logger.m_mutex };
        temp_filters = logger.m_openGL_filters;
    }
    logger.Output(std::move(output.str()), std::move(temp_filters));
}

void tilia::log::Logger::GLFW_Error_Callback(std::int32_t error_code, const char* description)
{
    std::stringstream output{};
    Logger& logger{ Logger::Instance() };
    output << "GLFW error"
        << "\nError Code: " << error_code
        << "\nDescription: " << description << '\n';
    logger.Output(std::move(output.str()), logger.m_openGL_filters);
}

#if TILIA_UNIT_TESTS == 1

// Vendor
#include "vendor/glad/KHR_Debug_openGL_3_3/include/glad/glad.h"
#include "vendor/glfw/include/GLFW/glfw3.h"
#include "vendor/Catch2/Catch2.hpp"

void tilia::log::Logger::Test()
{

    Logger& logger{ Logger::Instance() };

    

}

#endif