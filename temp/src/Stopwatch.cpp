/*****************************************************************//**
 * @file   Stopwatch.cpp
 * @brief  Defines functions for @Stopwatch class in @include "headers/Stopwatch.h"
 * 
 * Dependencies:
 * @include "dependencies/glfw/include/GLFW/glfw3.h"
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

// Dependencies
#include "dependencies/glfw/include/GLFW/glfw3.h"

// Standard
#include <iostream>

// Headers
#include "headers/Stopwatch.h"

/**
 * Gets the time values and if specified prints some values.
 */
float tilia::utils::Stopwatch::operator()(const Mode& mode, const std::string& message, bool should_print)
{
	std::pair<float, float> times{};
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

std::pair<float, float> tilia::utils::Stopwatch::Start()
{
	m_start_time = static_cast<float>(glfwGetTime());
	return { static_cast<float>(glfwGetTime()), 0.0f };
}

std::pair<float, float> tilia::utils::Stopwatch::Stop()
{
	return { static_cast<float>(glfwGetTime()), static_cast<float>(glfwGetTime()) - m_start_time };
}
