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
	window.m_title = std::move(std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Title::Get_Property(Window& window)
{
	std::get<0>(m_get_parameters) = window.m_title;
}

void tilia::windowing::properties::Iconify::Set_Property(Window& window)
{
	if (std::get<0>(m_set_parameters) == true)
	{
		glfwIconifyWindow(window.Get_Window());
	}
	else if(glfwGetWindowAttrib(window.Get_Window(), GLFW_ICONIFIED) != 0)
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
	else if (glfwGetWindowAttrib(window.Get_Window(), GLFW_MAXIMIZED) != 0)
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

void tilia::windowing::properties::Focus::Set_Property(Window& window)
{
	glfwFocusWindow(window.Get_Window());
}

void tilia::windowing::properties::Focus::Get_Property(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get_Window(), GLFW_FOCUSED);
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

void tilia::windowing::properties::Resizeable::Set_Property(Window& window)
{
	glfwSetWindowAttrib(window.Get_Window(), GLFW_RESIZABLE, std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Resizeable::Get_Property(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get_Window(), GLFW_RESIZABLE);
}

void tilia::windowing::properties::Decorated::Set_Property(Window& window)
{
	glfwSetWindowAttrib(window.Get_Window(), GLFW_DECORATED, std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Decorated::Get_Property(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get_Window(), GLFW_DECORATED);
}

void tilia::windowing::properties::Auto_Iconify::Set_Property(Window& window)
{
	glfwSetWindowAttrib(window.Get_Window(), GLFW_AUTO_ICONIFY, std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Auto_Iconify::Get_Property(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get_Window(), GLFW_AUTO_ICONIFY);
}

void tilia::windowing::properties::Floating::Set_Property(Window& window)
{
	glfwSetWindowAttrib(window.Get_Window(), GLFW_FLOATING, std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Floating::Get_Property(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get_Window(), GLFW_FLOATING);
}

void tilia::windowing::properties::Focus_On_Show::Set_Property(Window& window)
{
	glfwSetWindowAttrib(window.Get_Window(), GLFW_FOCUS_ON_SHOW, std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Focus_On_Show::Get_Property(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get_Window(), GLFW_FOCUS_ON_SHOW);
}

void tilia::windowing::properties::Transparent_Framebuffer::Get_Property(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get_Window(), GLFW_TRANSPARENT_FRAMEBUFFER);
}

void tilia::windowing::properties::context::Client_API::Get_Property(Window& window)
{

}

void tilia::windowing::properties::context::Creation_API::Get_Property(Window& window)
{
}

void tilia::windowing::properties::context::Version::Get_Property(Window& window)
{
}

void tilia::windowing::properties::context::OpenGL_Forward_Compatible::Get_Property(Window& window)
{
}

void tilia::windowing::properties::context::OpenGL_Debug_Context::Get_Property(Window& window)
{
}

void tilia::windowing::properties::context::OpenGL_Profile::Get_Property(Window& window)
{
}

void tilia::windowing::properties::context::Release_Behavior::Get_Property(Window& window)
{
}

void tilia::windowing::properties::context::No_Error::Get_Property(Window& window)
{
}

void tilia::windowing::properties::context::Robustness::Get_Property(Window& window)
{
}
