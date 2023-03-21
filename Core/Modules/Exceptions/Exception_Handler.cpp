// Tilia
#include "Exception_Handler.hpp"
#include "Core/Values/Directories.hpp"
#include TILIA_LOGGING_INCLUDE

void tilia::utils::Exception_Handler::Throw(const Tilia_Exception& exception)
{
	std::lock_guard lock{ m_mutex };
	m_tilia_exceptions.push_back(exception);
}

void tilia::utils::Exception_Handler::Throw(Tilia_Exception&& exception)
{
	std::lock_guard lock{ m_mutex };
	m_tilia_exceptions.push_back(std::move(exception));
}

void tilia::utils::Exception_Handler::Throw(const std::exception& exception)
{
	std::lock_guard lock{ m_mutex };
	m_exceptions.push_back(exception);
}

void tilia::utils::Exception_Handler::Throw(std::exception&& exception)
{
	std::lock_guard lock{ m_mutex };
	m_exceptions.push_back(std::move(exception));
}

void tilia::utils::Exception_Handler::Update()
{
	std::lock_guard lock{ m_mutex };

	log::Logger& logger{ log::Logger::Instance() };

	static auto print_exceptions{ [&logger](const auto& exceptions)
	{
		std::size_t i{ 1 };
		for (auto& exception : exceptions)
		{
			logger.Output("Exception #", i, ":\n\n", exception.what());
			++i;
		}
	} };

	if (m_tilia_exceptions.size() > 0)
	{
		logger.Output("Tilia Exceptions:\n\n");

		print_exceptions(m_tilia_exceptions);
	}
	if (m_exceptions.size() > 0)
	{
		logger.Output("Exceptions:\n\n");

		print_exceptions(m_exceptions);
	}

	m_tilia_exceptions.clear();
	m_exceptions.clear();

}

#if TILIA_UNIT_TESTS == 1

// Vendor
#include "vendor/Catch2/Catch2.hpp"

// Standard
#include <thread>

#define INT_VALUE 123
#define FLOAT_VALUE 0.15f
#define DOUBLE_VALUE 3.14
#define BOOL_VALUE true
#define STRING_VALUE "Hello World!"

static tilia::utils::Exception_Handler& handler{ tilia::utils::Exception_Handler::Instance() };

void thread_func_0()
{
	handler.Throw(tilia::utils::Tilia_Exception{ { TILIA_LOCATION,
		"Some error message about stuff and whatever...", INT_VALUE, FLOAT_VALUE,
		DOUBLE_VALUE, BOOL_VALUE, STRING_VALUE } });
}

void tilia::utils::Exception_Handler::Test()
{

	log::Logger& logger{ log::Logger::Instance() };

	// Test for Exception_Handler::Instance() returning correct address

	REQUIRE(&handler == &Exception_Handler::Instance());

	// Test throw function with copy tilia exception

	tilia::utils::Tilia_Exception t_e_0{ { TILIA_LOCATION,
		"Some error message about stuff and whatever...", INT_VALUE, FLOAT_VALUE,
		DOUBLE_VALUE, BOOL_VALUE, STRING_VALUE } };

	handler.Throw(t_e_0);

	REQUIRE(handler.m_tilia_exceptions.size() == 1);
	REQUIRE(handler.m_tilia_exceptions[0] == t_e_0);

	// Test throw function with move tilia exception

	handler.Throw(std::move(t_e_0));

	REQUIRE(handler.m_tilia_exceptions.size() == 2);
	REQUIRE(handler.m_tilia_exceptions[1] == handler.m_tilia_exceptions[0]);

	// Test throw function with copy general exception

	std::exception g_e_0{ "Some error message about stuff and whatever..." };

	handler.Throw(g_e_0);

	REQUIRE(handler.m_exceptions.size() == 1);
	REQUIRE(strcmp(handler.m_exceptions[0].what(), g_e_0.what()) == 0);

}

#endif // TILIA_UNIT_TESTS == 1