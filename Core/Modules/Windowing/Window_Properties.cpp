// Vendor
#include "vendor/glad/KHR_Debug_openGL_3_3/include/glad/glad.h"
#include "vendor/glfw/include/GLFW/glfw3.h"

// Standard
#include <iostream>

// Tilia
#include "Window_Properties.hpp"
#include "Core/Values/Directories.hpp"
#include TILIA_WINDOW_INCLUDE

void tilia::windowing::properties::Should_Close::Set_Property(Window& window)
{
	glfwSetWindowShouldClose(window.Get_Window(), std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Should_Close::Get_Property(Window& window)
{
	std::get<0>(m_get_parameters) = glfwWindowShouldClose(window.Get_Window());
}

void tilia::windowing::properties::Size::Set_Property(Window& window)
{
	glfwSetWindowSize(window.Get_Window(), std::get<0>(m_set_parameters), std::get<1>(m_set_parameters));
}

void tilia::windowing::properties::Size::Get_Property(Window& window)
{
	glfwGetWindowSize(window.Get_Window(), &std::get<0>(m_get_parameters), &std::get<1>(m_get_parameters));
}

void tilia::windowing::properties::Frame_Size::Get_Property(Window& window)
{
	glfwGetWindowFrameSize(window.Get_Window(), &std::get<0>(m_get_parameters), &std::get<1>(m_get_parameters),
		&std::get<2>(m_get_parameters), &std::get<3>(m_get_parameters));
}

void tilia::windowing::properties::Framebuffer_Size::Get_Property(Window& window)
{
	glfwGetFramebufferSize(window.Get_Window(), &std::get<0>(m_get_parameters), &std::get<1>(m_get_parameters));
}

void tilia::windowing::properties::Content_Scale::Get_Property(Window& window)
{
	glfwGetWindowContentScale(window.Get_Window(), &std::get<0>(m_get_parameters), &std::get<1>(m_get_parameters));
}

void tilia::windowing::properties::Size_Limits::Set_Property(Window& window)
{
	glfwSetWindowSizeLimits(window.Get_Window(), std::get<0>(m_set_parameters), std::get<1>(m_set_parameters),
		std::get<2>(m_set_parameters), std::get<3>(m_set_parameters));
}

void tilia::windowing::properties::Aspect_Ratio::Set_Property(Window& window)
{
	glfwSetWindowAspectRatio(window.Get_Window(), std::get<0>(m_set_parameters), std::get<1>(m_set_parameters));
}

void tilia::windowing::properties::Position::Set_Property(Window& window)
{
	glfwSetWindowPos(window.Get_Window(), std::get<0>(m_set_parameters), std::get<1>(m_set_parameters));
}

void tilia::windowing::properties::Position::Get_Property(Window& window)
{
	glfwGetWindowPos(window.Get_Window(), &std::get<0>(m_get_parameters), &std::get<1>(m_get_parameters));
}

void tilia::windowing::properties::Title::Set_Property(Window& window)
{
	glfwSetWindowTitle(window.Get_Window(), std::get<0>(m_set_parameters).c_str());
}

void tilia::windowing::properties::Iconify::Set_Property(Window& window)
{
	if (std::get<0>(m_set_parameters) == true)
	{
		glfwIconifyWindow(window.Get_Window());
	}
	else
	{
		glfwRestoreWindow(window.Get_Window());
	}
}

void tilia::windowing::properties::Iconify::Get_Property(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get_Window(), GLFW_ICONIFIED);
}

void tilia::windowing::properties::Maximize::Set_Property(Window& window)
{
	if (std::get<0>(m_set_parameters) == true)
	{
		glfwMaximizeWindow(window.Get_Window());
	}
	else
	{
		glfwRestoreWindow(window.Get_Window());
	}
}

void tilia::windowing::properties::Maximize::Get_Property(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get_Window(), GLFW_MAXIMIZED);
}

void tilia::windowing::properties::Visible::Set_Property(Window& window)
{
	if (std::get<0>(m_set_parameters) == true)
	{
		glfwShowWindow(window.Get_Window());
	}
	else
	{
		glfwHideWindow(window.Get_Window());
	}
}

void tilia::windowing::properties::Visible::Get_Property(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get_Window(), GLFW_VISIBLE);
}

void tilia::windowing::properties::Request_Attention::Set_Property(Window& window)
{
	glfwRequestWindowAttention(window.Get_Window());
}

void tilia::windowing::properties::Opacity::Set_Property(Window& window)
{
	glfwSetWindowOpacity(window.Get_Window(), std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Opacity::Get_Property(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowOpacity(window.Get_Window());
}

void tilia::windowing::properties::Swap_Interval::Set_Property(Window& window)
{
	window.m_swap_interval = std::get<0>(m_set_parameters);
	if (window.m_window == glfwGetCurrentContext())
	{
		glfwSwapInterval(window.m_swap_interval);
	}
}

void tilia::windowing::properties::Swap_Interval::Get_Property(Window& window)
{
	std::get<0>(m_get_parameters) = window.m_swap_interval;
}
