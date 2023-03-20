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
            if (*i == *k)
                return true;
        }
    }
    return false;
}

void tilia::log::Logger::Output(const std::string& data, 
    const std::vector<std::string>& filters)
{
    std::lock_guard lock{ m_mutex };
    if (m_outputs.size() == 0)
        throw utils::Tilia_Exception{ { TILIA_LOCATION, "Logger has no viable output\n" } };

    const auto& logger_filters{ (filters.size() == 0) ? m_filters : filters };
    for (auto& [output, output_filters] : m_outputs)
    {
        if (shares_filters(output_filters, logger_filters) || logger_filters.size() == 0 
            || output_filters.size() == 0)
        {
            std::ostream{ output } << data;
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

void tilia::log::Logger::Remove_Output(std::streambuf* const output)
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
    if (i < m_outputs.cend())
        m_outputs.erase(i);
}

void tilia::log::Logger::Set_Output_Filters(std::streambuf* output,
    const std::vector<std::string>& filters)
{
    std::lock_guard lock{ m_mutex };
    for (auto i{ m_outputs.begin() }; i < m_outputs.end(); ++i)
    {
        if (output == i->first)
        {
            i->second = filters;
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
    logger.Output(std::move(output.str()), logger.m_GLFW_filters);
}

#if TILIA_UNIT_TESTS == 1

// Vendor
#include "vendor/glad/KHR_Debug_openGL_3_3/include/glad/glad.h"
#include "vendor/glfw/include/GLFW/glfw3.h"
#include "vendor/Catch2/Catch2.hpp"

#define INT_VALUE 123
#define FLOAT_VALUE 0.15f
#define DOUBLE_VALUE 3.14
#define BOOL_VALUE true
#define STRING_VALUE "Hello World!"

void tilia::log::Logger::Test()
{

    std::stringstream additional_values_0{};
    additional_values_0 << INT_VALUE << FLOAT_VALUE << DOUBLE_VALUE << BOOL_VALUE << STRING_VALUE;

    // Test for Logger::Instance() returning correct address

    Logger& logger{ Logger::Instance() };

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(Logger::OpenGL_Error_Callback, nullptr);
    glfwSetErrorCallback(Logger::GLFW_Error_Callback);

    REQUIRE(&logger == &Logger::Instance());

    std::stringbuf my_buffer_0{};

    // Test for adding an output

    logger.Add_Output(&my_buffer_0);

    REQUIRE(logger.Get_Output_Count() == 1);
    REQUIRE(logger.Get_Output(0).first == &my_buffer_0);

    // Test for output with message

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE, 
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_0.str() == "My test output" + additional_values_0.str());

    my_buffer_0.str("");

    REQUIRE(my_buffer_0.str() == "");

    // Test filter comparison function

    REQUIRE(shares_filters({ "test_filter_0" }, { "test_filter_0" }));
    REQUIRE(shares_filters({ "test_filter_0" }, { "test_filter_0", "test_filter_1" }));
    REQUIRE(shares_filters({ "test_filter_0", "test_filter_1" }, { "test_filter_0" }));
    REQUIRE_FALSE(shares_filters({ "test_filter_0" }, { "test_filter_1" }));
    REQUIRE_FALSE(shares_filters({ "test_filter_0" }, { "test_filter_1", "test_filter_2" }));
    REQUIRE_FALSE(shares_filters({ "test_filter_0", "test_filter_2" }, { "test_filter_1" }));

    // Test for filters where output has none but logger does

    logger.Set_Filters({ "test_filter_0" });

    REQUIRE(logger.Get_Filters() == std::vector<std::string>{ "test_filter_0" });

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE,
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_0.str() == "My test output" + additional_values_0.str());

    my_buffer_0.str("");

    // Test for filters where both output and logger have same filter

    logger.Set_Output_Filters(&my_buffer_0, { "test_filter_0" });

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE,
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_0.str() == "My test output" + additional_values_0.str());

    my_buffer_0.str("");

    // Test for filters where logger has none but output does

    logger.Set_Filters();

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE,
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_0.str() == "My test output" + additional_values_0.str());

    my_buffer_0.str("");

    // Test for filters where output and logger have different filters

    logger.Set_Filters({ "test_filter_1" });

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE,
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_0.str() == "");

    // Test for filters where output has same as one of loggers filters

    logger.Set_Filters({ "test_filter_0", "test_filter_1" });

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE,
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_0.str() == "My test output" + additional_values_0.str());

    my_buffer_0.str("");

    // Test for filters where logger has same as one of outputs filters

    logger.Set_Filters({ "test_filter_0" });

    logger.Set_Output_Filters(&my_buffer_0, { "test_filter_0", "test_filter_1" });

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE,
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_0.str() == "My test output" + additional_values_0.str());

    my_buffer_0.str("");

    // Test for filters and multiple outputs where two outputs have different filters and logger
    // has both the filters

    logger.Set_Filters({ "test_filter_0", "test_filter_1" });
    logger.Set_Output_Filters(&my_buffer_0, { "test_filter_0" });

    std::stringbuf my_buffer_1{};

    logger.Add_Output(&my_buffer_1, { "test_filter_1" });

    REQUIRE(logger.Get_Output_Count() == 2);
    REQUIRE(logger.Get_Output(1).first == &my_buffer_1);

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE,
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_1.str() == "My test output" + additional_values_0.str());
    REQUIRE(my_buffer_0.str() == my_buffer_1.str());

    // Test for removing output

    logger.Remove_Output(&my_buffer_1);

    REQUIRE(logger.Get_Output_Count() == 1);

    logger.Set_Filters();

    my_buffer_0.str("");

    // Test for outputting exception data

    utils::Exception_Data e_d_0{ TILIA_LOCATION, "My test output", INT_VALUE, FLOAT_VALUE, 
        DOUBLE_VALUE, BOOL_VALUE, STRING_VALUE };

    logger.Output(e_d_0);

    REQUIRE(my_buffer_0.str() != "");

    my_buffer_0.str("");

    // Test for outputting tilia exception

    utils::Tilia_Exception t_e_0{ e_d_0 };

    logger.Output(t_e_0);

    REQUIRE(my_buffer_0.str() != "");

    my_buffer_0.str("");

    // Test for outputting with manually giving filters

    logger.Output("My test output", {});

    REQUIRE(my_buffer_0.str() == "My test output");

    my_buffer_0.str("");

    logger.Set_Output_Filters(&my_buffer_0);

    logger.Output("My test output", { "test_filter_0 "});

    REQUIRE(my_buffer_0.str() == "My test output");

    my_buffer_0.str("");

    // Test for openGL callback

    // Call in different scope to ensure callback is called
    {
        glEnable(GL_FALSE);
    }

    REQUIRE(my_buffer_0.str() != "");

    my_buffer_0.str("");

    // Test for openGL callback filtering where both the output filters and the openGL filters are
    // the same

    logger.Set_Output_Filters(&my_buffer_0, { "test_filter_0" });
    logger.Set_OpenGL_Filters({ "test_filter_0" });

    // Call in different scope to ensure callback is called
    {
        glEnable(GL_FALSE);
    }

    REQUIRE(my_buffer_0.str() != "");

    my_buffer_0.str("");

    // Test for openGL callback filtering where they are different

    logger.Set_Output_Filters(&my_buffer_0, { "test_filter_0" });
    logger.Set_OpenGL_Filters({ "test_filter_1" });

    // Call in different scope to ensure callback is called
    {
        glEnable(GL_FALSE);
    }

    REQUIRE(my_buffer_0.str() == "");

    my_buffer_0.str("");
    
    // Test for GLFW callback

    logger.Set_Output_Filters(&my_buffer_0);

    glfwWindowHint(999999, 888888);

    REQUIRE(my_buffer_0.str() != "");

    my_buffer_0.str("");

    // Test for GLFW callback filtering where both the output filters and the GLFW filters are
    // the same

    logger.Set_Output_Filters(&my_buffer_0, { "test_filter_0" });
    logger.Set_GLFW_Filters({ "test_filter_0" });

    glfwWindowHint(999999, 888888);

    REQUIRE(my_buffer_0.str() != "");

    my_buffer_0.str("");

    // Test for GLFW callback filtering where they are different

    logger.Set_Output_Filters(&my_buffer_0, { "test_filter_0" });
    logger.Set_GLFW_Filters({ "test_filter_1" });

    glfwWindowHint(999999, 888888);

    REQUIRE(my_buffer_0.str() == "");

    my_buffer_0.str("");

}

#endif // TILIA_UNIT_TESTS == 1