/*****************************************************************//**
 * @file   Stopwatch.cpp
 * @brief  Defines functions for @Stopwatch class in @include "headers/Stopwatch.h"
 * 
 * Standard:
 * @include <iostream>
 * 
 * Headers:
 * @include "headers/Stopwatch.h"
 * 
 * @author Gustav Fagerlind
 * @date   30/06/2022
 *********************************************************************/

// Standard
#include <iostream>
#include <chrono>

// Headers
#include "Core/Temp/Stopwatch.hpp"

/**
 * Gets the time values and if specified prints some values.
 */
int64_t tilia::utils::Stopwatch::operator()(const Mode& mode, const std::string& message, bool should_print)
{
	std::pair<int64_t, int64_t> times{};
	switch (mode)
	{
	case Mode::Start:
		times = Start();
		if (should_print)
			std::cout << "Start :: ";
		break;
	case Mode::Stop:
		times = Stop();
		if (should_print)
			std::cout << "Stop :: ";
		break;
	}
	if (should_print)
		std::cout << "Total Time: " << times.first << " :: Stopwatch Time: " << times.second << " :: " << message << '\n';
	return times.second;
}

std::pair<int64_t, int64_t> tilia::utils::Stopwatch::Start()
{
	auto time{ std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) };

	m_start_time = time.count();
	return { time.count(), 0 };
}

std::pair<int64_t, int64_t> tilia::utils::Stopwatch::Stop()
{
	auto time{ std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) };

	return { time.count(), time.count() - m_start_time };
}
