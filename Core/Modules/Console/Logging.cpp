// Standard
#include <iostream>
#include <algorithm>

// Tilia
#include "Logging.hpp"
#include "Core/Values/Directories.hpp"
#include TILIA_EXCEPTION_HANDLER_INCLUDE

static bool shares_filters(const std::set<std::string>& lhs,
    const std::set<std::string>& rhs)
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
    const std::set<std::string>& filters)
{
    std::lock_guard lock{ m_mutex };
    if (m_outputs.size() == 0)
        return;
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
    auto potential_message{ [](const auto& message) -> const char*
    {
        if (message.size() > 0)
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

static tilia::log::Logger& logger{ tilia::log::Logger::Instance() };

void tilia::log::Logger::Test()
{

    std::stringstream additional_values_0{};
    additional_values_0 << INT_VALUE << FLOAT_VALUE << DOUBLE_VALUE << BOOL_VALUE << STRING_VALUE;

    std::stringbuf my_buffer_0{};
    std::stringbuf my_buffer_1{};

    const utils::Exception_Data e_d_0{ TILIA_LOCATION, "My test output", INT_VALUE, FLOAT_VALUE,
    DOUBLE_VALUE, BOOL_VALUE, STRING_VALUE };

    const utils::Tilia_Exception t_e_0{ e_d_0 };

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(Logger::OpenGL_Error_Callback, nullptr);
    glfwSetErrorCallback(Logger::GLFW_Error_Callback);

    // Test for Logger::Instance() returning correct address

    REQUIRE(&logger == &Logger::Instance());

    // Test for adding an output

    logger.Add_Output(&my_buffer_0);

    REQUIRE(logger.Get_Output_Count() == 1);
    REQUIRE(logger.Get_Output(0).first == &my_buffer_0);

    // Test for output with message

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE, 
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_0.str() == "My test output" + additional_values_0.str());

    // Test filter comparison function

    REQUIRE(shares_filters({ "test_filter_0" }, { "test_filter_0" }));
    REQUIRE(shares_filters({ "test_filter_0" }, { "test_filter_0", "test_filter_1" }));
    REQUIRE(shares_filters({ "test_filter_0", "test_filter_1" }, { "test_filter_0" }));
    REQUIRE_FALSE(shares_filters({ "test_filter_0" }, { "test_filter_1" }));
    REQUIRE_FALSE(shares_filters({ "test_filter_0" }, { "test_filter_1", "test_filter_2" }));
    REQUIRE_FALSE(shares_filters({ "test_filter_0", "test_filter_2" }, { "test_filter_1" }));

    // Test for filters where both output and logger have same filter

    my_buffer_0.str("");

    logger.Set_Filters({ "test_filter_0" });
    logger.Set_Output_Filters(&my_buffer_0, { "test_filter_0" });

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE,
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_0.str() == "My test output" + additional_values_0.str());

    // Test for filters where output has none but logger does

    my_buffer_0.str("");

    logger.Set_Filters({ "test_filter_0" });
    logger.Set_Output_Filters(&my_buffer_0, {});

    REQUIRE(logger.Get_Filters() == std::vector<std::string>{ "test_filter_0" });

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE,
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_0.str() == "My test output" + additional_values_0.str());

    // Test for filters where logger has none but output does

    my_buffer_0.str("");

    logger.Set_Filters({});
    logger.Set_Output_Filters(&my_buffer_0, { "test_filter_0" });

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE,
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_0.str() == "My test output" + additional_values_0.str());

    // Test for filters where output and logger have different filters

    my_buffer_0.str("");

    logger.Set_Filters({ "test_filter_1" });
    logger.Set_Output_Filters(&my_buffer_0, { "test_filter_0" });

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE,
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_0.str() == "");

    // Test for filters where output has same as one of loggers filters

    my_buffer_0.str("");

    logger.Set_Filters({ "test_filter_0", "test_filter_1" });
    logger.Set_Output_Filters(&my_buffer_0, { "test_filter_0" });

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE,
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_0.str() == "My test output" + additional_values_0.str());

    // Test for filters where logger has same as one of outputs filters

    my_buffer_0.str("");

    logger.Set_Filters({ "test_filter_0" });
    logger.Set_Output_Filters(&my_buffer_0, { "test_filter_0", "test_filter_1" });

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE,
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_0.str() == "My test output" + additional_values_0.str());

    // Test for filters and multiple outputs where two outputs have different filters and logger
    // has both the filters

    my_buffer_0.str("");

    logger.Set_Filters({ "test_filter_0", "test_filter_1" });
    logger.Set_Output_Filters(&my_buffer_0, { "test_filter_0" });

    logger.Add_Output(&my_buffer_1, { "test_filter_1" });

    REQUIRE(logger.Get_Output_Count() == 2);
    REQUIRE(logger.Get_Output(1).first == &my_buffer_1);

    logger.Output("My test output", INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE,
        BOOL_VALUE, STRING_VALUE);

    REQUIRE(my_buffer_0.str() == "My test output" + additional_values_0.str());
    REQUIRE(my_buffer_1.str() == "My test output" + additional_values_0.str());

    // Test for removing output

    logger.Remove_Output(&my_buffer_0);

    REQUIRE(logger.Get_Output(0).first == &my_buffer_1);

    // Test for removing already removed output

    logger.Remove_Output(&my_buffer_0);

    REQUIRE(logger.Get_Output(0).first == &my_buffer_1);

    // Test for outputting exception data

    my_buffer_1.str("");

    logger.Set_Filters({});
    logger.Set_Output_Filters(&my_buffer_1, {});

    logger.Output(e_d_0);

    REQUIRE(my_buffer_1.str() != "");

    // Test for outputting tilia exception

    my_buffer_1.str("");

    logger.Set_Filters({});
    logger.Set_Output_Filters(&my_buffer_1, {});

    logger.Output(t_e_0);

    REQUIRE(my_buffer_1.str() != "");

    // Test for outputting with manually giving no filters

    my_buffer_1.str("");

    logger.Set_Filters({});
    logger.Set_Output_Filters(&my_buffer_1, {});

    logger.Output("My test output", {});

    REQUIRE(my_buffer_1.str() == "My test output");

    // Test for outputting with manually giving filters

    my_buffer_1.str("");

    logger.Set_Filters({});
    logger.Set_Output_Filters(&my_buffer_1, { "test_filter_0" });

    logger.Output("My test output", { "test_filter_0" });

    REQUIRE(my_buffer_1.str() == "My test output");

    // Test for openGL callback

    my_buffer_1.str("");

    logger.Set_OpenGL_Filters({});
    logger.Set_Output_Filters(&my_buffer_1, {});

    // Call in different scope to ensure callback is called
    {
        glEnable(GL_FALSE);
    }

    REQUIRE(my_buffer_1.str() != "");

    // Test for openGL callback filtering where both the output filters and the openGL filters are
    // the same

    my_buffer_1.str("");

    logger.Set_OpenGL_Filters({ "test_filter_0" });
    logger.Set_Output_Filters(&my_buffer_1, { "test_filter_0" });

    // Call in different scope to ensure callback is called
    {
        glEnable(GL_FALSE);
    }

    REQUIRE(my_buffer_1.str() != "");

    // Test for openGL callback filtering where they are different

    my_buffer_1.str("");

    logger.Set_OpenGL_Filters({ "test_filter_1" });
    logger.Set_Output_Filters(&my_buffer_1, { "test_filter_0" });

    // Call in different scope to ensure callback is called
    {
        glEnable(GL_FALSE);
    }

    REQUIRE(my_buffer_1.str() == "");
    
    // Test for GLFW callback

    my_buffer_1.str("");

    logger.Set_GLFW_Filters({});
    logger.Set_Output_Filters(&my_buffer_1, {});

    glfwWindowHint(999999, 888888);

    REQUIRE(my_buffer_1.str() != "");

    // Test for GLFW callback filtering where both the output filters and the GLFW filters are
    // the same

    my_buffer_1.str("");

    logger.Set_GLFW_Filters({ "test_filter_0" });
    logger.Set_Output_Filters(&my_buffer_1, { "test_filter_0" });

    glfwWindowHint(999999, 888888);

    REQUIRE(my_buffer_1.str() != "");

    // Test for GLFW callback filtering where they are different

    my_buffer_1.str("");

    logger.Set_GLFW_Filters({ "test_filter_1" });
    logger.Set_Output_Filters(&my_buffer_1, { "test_filter_0" });

    glfwWindowHint(999999, 888888);

    REQUIRE(my_buffer_1.str() == "");

    // Reset all values to default

    logger.Set_Filters({});
    logger.Set_OpenGL_Filters({});
    logger.Set_GLFW_Filters({});

    logger.Remove_Output(&my_buffer_0);
    logger.Remove_Output(&my_buffer_1);

    glDebugMessageCallback(nullptr, nullptr);
    glfwSetErrorCallback(nullptr);

    glDisable(GL_DEBUG_OUTPUT);
    glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

}

#endif // TILIA_UNIT_TESTS == 1