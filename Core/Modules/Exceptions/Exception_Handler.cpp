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

static tilia::utils::Exception_Handler& handler{ tilia::utils::Exception_Handler::Instance() };

void tilia::utils::Exception_Handler::Test()
{

	// Test for Exception_Handler::Instance() returning correct address

	REQUIRE(&handler == &Exception_Handler::Instance());



}

#endif // TILIA_UNIT_TESTS == 1