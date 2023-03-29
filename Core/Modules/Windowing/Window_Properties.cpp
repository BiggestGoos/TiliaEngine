// Vendor
#include "vendor/glad/KHR_Debug_openGL_3_3/include/glad/glad.h"
#include "vendor/glfw/include/GLFW/glfw3.h"

// Standard
#include <iostream>

// Tilia
#include "Window_Properties.hpp"
#include "Core/Values/Directories.hpp"
#include TILIA_WINDOW_INCLUDE

void tilia::windowing::hints::Set_Hint(std::int32_t type, std::int32_t value)
{
	glfwWindowHint(type, value);
}

void tilia::windowing::hints::Reset()
{
	glfwDefaultWindowHints();
}

void tilia::windowing::properties::Underlying_Window::Get(Window& window)
{
	std::get<0>(m_get_parameters) = window.m_window;
}

void tilia::windowing::properties::Should_Close::Set(Window& window)
{
	glfwSetWindowShouldClose(window.Get(Underlying_Window{}), std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Should_Close::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwWindowShouldClose(window.Get(Underlying_Window{}));
}

void tilia::windowing::properties::Size::Set(Window& window)
{
	glfwSetWindowSize(window.Get(Underlying_Window{}), std::get<0>(m_set_parameters), std::get<1>(m_set_parameters));
}

void tilia::windowing::properties::Size::Get(Window& window)
{
	glfwGetWindowSize(window.Get(Underlying_Window{}), &std::get<0>(m_get_parameters), &std::get<1>(m_get_parameters));
}

void tilia::windowing::properties::Frame_Size::Get(Window& window)
{
	glfwGetWindowFrameSize(window.Get(Underlying_Window{}), &std::get<0>(m_get_parameters), &std::get<1>(m_get_parameters),
		&std::get<2>(m_get_parameters), &std::get<3>(m_get_parameters));
}

void tilia::windowing::properties::Framebuffer_Size::Get(Window& window)
{
	glfwGetFramebufferSize(window.Get(Underlying_Window{}), &std::get<0>(m_get_parameters), &std::get<1>(m_get_parameters));
}

void tilia::windowing::properties::Content_Scale::Get(Window& window)
{
	glfwGetWindowContentScale(window.Get(Underlying_Window{}), &std::get<0>(m_get_parameters), &std::get<1>(m_get_parameters));
}

void tilia::windowing::properties::Size_Limits::Set(Window& window)
{
	glfwSetWindowSizeLimits(window.Get(Underlying_Window{}), std::get<0>(m_set_parameters), std::get<1>(m_set_parameters),
		std::get<2>(m_set_parameters), std::get<3>(m_set_parameters));
}

void tilia::windowing::properties::Aspect_Ratio::Set(Window& window)
{
	glfwSetWindowAspectRatio(window.Get(Underlying_Window{}), std::get<0>(m_set_parameters), std::get<1>(m_set_parameters));
}

void tilia::windowing::properties::Position::Set(Window& window)
{
	glfwSetWindowPos(window.Get(Underlying_Window{}), std::get<0>(m_set_parameters), std::get<1>(m_set_parameters));
}

void tilia::windowing::properties::Position::Get(Window& window)
{
	glfwGetWindowPos(window.Get(Underlying_Window{}), &std::get<0>(m_get_parameters), &std::get<1>(m_get_parameters));
}

void tilia::windowing::properties::Title::Set(Window& window)
{
	glfwSetWindowTitle(window.Get(Underlying_Window{}), std::get<0>(m_set_parameters).c_str());
	window.m_title = std::move(std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Title::Get(Window& window)
{
	std::get<0>(m_get_parameters) = window.m_title;
}

void tilia::windowing::properties::Iconify::Set(Window& window)
{
	if (std::get<0>(m_set_parameters) == true)
	{
		glfwIconifyWindow(window.Get(Underlying_Window{}));
	}
	else if(glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_ICONIFIED) != 0)
	{
		glfwRestoreWindow(window.Get(Underlying_Window{}));
	}
}

void tilia::windowing::properties::Iconify::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_ICONIFIED);
}

void tilia::windowing::properties::Maximize::Set(Window& window)
{
	if (std::get<0>(m_set_parameters) == true)
	{
		glfwMaximizeWindow(window.Get(Underlying_Window{}));
	}
	else if (glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_MAXIMIZED) != 0)
	{
		glfwRestoreWindow(window.Get(Underlying_Window{}));
	}
}

void tilia::windowing::properties::Maximize::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_MAXIMIZED);
}

void tilia::windowing::properties::Visible::Set(Window& window)
{
	if (std::get<0>(m_set_parameters) == true)
	{
		glfwShowWindow(window.Get(Underlying_Window{}));
	}
	else
	{
		glfwHideWindow(window.Get(Underlying_Window{}));
	}
}

void tilia::windowing::properties::Visible::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_VISIBLE);
}

void tilia::windowing::properties::Focus::Set(Window& window)
{
	glfwFocusWindow(window.Get(Underlying_Window{}));
}

void tilia::windowing::properties::Focus::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_FOCUSED);
}

void tilia::windowing::properties::Request_Attention::Set(Window& window)
{
	glfwRequestWindowAttention(window.Get(Underlying_Window{}));
}

void tilia::windowing::properties::Opacity::Set(Window& window)
{
	glfwSetWindowOpacity(window.Get(Underlying_Window{}), std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Opacity::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowOpacity(window.Get(Underlying_Window{}));
}

void tilia::windowing::properties::Swap_Interval::Set(Window& window)
{
	window.m_swap_interval = std::get<0>(m_set_parameters);
	if (window.m_window == glfwGetCurrentContext())
	{
		glfwSwapInterval(window.m_swap_interval);
	}
}

void tilia::windowing::properties::Swap_Interval::Get(Window& window)
{
	std::get<0>(m_get_parameters) = window.m_swap_interval;
}

void tilia::windowing::properties::Resizeable::Set(Window& window)
{
	glfwSetWindowAttrib(window.Get(Underlying_Window{}), GLFW_RESIZABLE, std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Resizeable::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_RESIZABLE);
}

void tilia::windowing::properties::Decorated::Set(Window& window)
{
	glfwSetWindowAttrib(window.Get(Underlying_Window{}), GLFW_DECORATED, std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Decorated::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_DECORATED);
}

void tilia::windowing::properties::Auto_Iconify::Set(Window& window)
{
	glfwSetWindowAttrib(window.Get(Underlying_Window{}), GLFW_AUTO_ICONIFY, std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Auto_Iconify::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_AUTO_ICONIFY);
}

void tilia::windowing::properties::Floating::Set(Window& window)
{
	glfwSetWindowAttrib(window.Get(Underlying_Window{}), GLFW_FLOATING, std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Floating::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_FLOATING);
}

void tilia::windowing::properties::Focus_On_Show::Set(Window& window)
{
	glfwSetWindowAttrib(window.Get(Underlying_Window{}), GLFW_FOCUS_ON_SHOW, std::get<0>(m_set_parameters));
}

void tilia::windowing::properties::Focus_On_Show::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_FOCUS_ON_SHOW);
}

void tilia::windowing::properties::Transparent_Framebuffer::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_TRANSPARENT_FRAMEBUFFER);
}

void tilia::windowing::properties::context::Client_API::Get(Window& window)
{
	std::get<0>(m_get_parameters) = static_cast<enums::Client_API>(glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_CLIENT_API));
}

void tilia::windowing::properties::context::Creation_API::Get(Window& window)
{
	std::get<0>(m_get_parameters) = static_cast<enums::Context_Creation_API>(glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_CONTEXT_CREATION_API));
}

void tilia::windowing::properties::context::Version::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_CONTEXT_VERSION_MAJOR);
	std::get<1>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_CONTEXT_VERSION_MINOR);
	std::get<2>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_CONTEXT_REVISION);
}

void tilia::windowing::properties::context::OpenGL_Forward_Compatible::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_OPENGL_FORWARD_COMPAT);
}

void tilia::windowing::properties::context::OpenGL_Debug_Context::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_OPENGL_DEBUG_CONTEXT);
}

void tilia::windowing::properties::context::OpenGL_Profile::Get(Window& window)
{
	std::get<0>(m_get_parameters) = static_cast<enums::OpenGL_Profile>(glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_OPENGL_PROFILE));
}

void tilia::windowing::properties::context::Release_Behavior::Get(Window& window)
{
	std::get<0>(m_get_parameters) = static_cast<enums::Context_Release_Behavior>(glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_CONTEXT_RELEASE_BEHAVIOR));
}

void tilia::windowing::properties::context::No_Error::Get(Window& window)
{
	std::get<0>(m_get_parameters) = glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_CONTEXT_NO_ERROR);
}

void tilia::windowing::properties::context::Robustness::Get(Window& window)
{
	std::get<0>(m_get_parameters) = static_cast<enums::Context_Robustness>(glfwGetWindowAttrib(window.Get(Underlying_Window{}), GLFW_CONTEXT_ROBUSTNESS));
}

void tilia::windowing::properties::Set_Window_Property<tilia::enums::Window_Properties::Should_Close>::Set(Window& window, Tuple tuple)
{
	glfwSetWindowShouldClose(window.Get(windowing::properties::Underlying_Window{}), std::get<0>(tuple));
}