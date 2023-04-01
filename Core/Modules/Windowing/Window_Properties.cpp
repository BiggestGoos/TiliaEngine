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

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Should_Close>::Set(Window& window, Set_Parameters&& parameters)
{
	glfwSetWindowShouldClose(window.Get<enums::Window_Properties::Underlying_Window>(), parameters);
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Should_Close>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Should_Close>::Get(Window& window)
{
	return glfwWindowShouldClose(window.Get<enums::Window_Properties::Underlying_Window>());
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Size>::Set(Window& window, Set_Parameters&& parameters)
{
	glfwSetWindowSize(window.Get<enums::Window_Properties::Underlying_Window>(), std::get<0>(parameters), std::get<1>(parameters));
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Size>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Size>::Get(Window& window)
{
	Get_Parameters ret_val{};
	glfwGetWindowSize(window.Get<enums::Window_Properties::Underlying_Window>(), &std::get<0>(ret_val), &std::get<1>(ret_val));
	return ret_val;
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Frame_Size>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Frame_Size>::Get(Window& window)
{
	Get_Parameters ret_val{};
	glfwGetWindowFrameSize(window.Get<enums::Window_Properties::Underlying_Window>(), &std::get<0>(ret_val), &std::get<1>(ret_val), &std::get<2>(ret_val), &std::get<3>(ret_val));
	return ret_val;
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Framebuffer_Size>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Framebuffer_Size>::Get(Window& window)
{
	Get_Parameters ret_val{};
	glfwGetFramebufferSize(window.Get<enums::Window_Properties::Underlying_Window>(), &std::get<0>(ret_val), &std::get<1>(ret_val));
	return ret_val;
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Content_Scale>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Content_Scale>::Get(Window& window)
{
	Get_Parameters ret_val{};
	glfwGetWindowContentScale(window.Get<enums::Window_Properties::Underlying_Window>(), &std::get<0>(ret_val), &std::get<1>(ret_val));
	return ret_val;
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Size_Limits>::Set(Window& window, Set_Parameters&& parameters)
{
	glfwSetWindowSizeLimits(window.Get<enums::Window_Properties::Underlying_Window>(), std::get<0>(parameters), std::get<1>(parameters), std::get<2>(parameters), std::get<3>(parameters));
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Aspect_Ratio>::Set(Window& window, Set_Parameters&& parameters)
{
	glfwSetWindowAspectRatio(window.Get<enums::Window_Properties::Underlying_Window>(), std::get<0>(parameters), std::get<1>(parameters));
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Position>::Set(Window& window, Set_Parameters&& parameters)
{
	glfwSetWindowPos(window.Get<enums::Window_Properties::Underlying_Window>(), std::get<0>(parameters), std::get<1>(parameters));
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Position>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Position>::Get(Window& window)
{
	Get_Parameters ret_val{};
	glfwGetWindowPos(window.Get<enums::Window_Properties::Underlying_Window>(), &std::get<0>(ret_val), &std::get<1>(ret_val));
	return ret_val;
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Title>::Set(Window& window, Set_Parameters&& parameters)
{
	glfwSetWindowTitle(window.Get<enums::Window_Properties::Underlying_Window>(), parameters.c_str());
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Title>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Title>::Get(Window& window)
{
	return window.m_title;
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Monitor>::Set(Window& window, Set_Parameters&& parameters)
{
	auto underlying_window{ window.Get<enums::Window_Properties::Underlying_Window>() };
	auto underlying_monitor{ parameters.Get<enums::Monitor_Properties::Underlying_Monitor>() };
	auto [res_x, res_y] { parameters.Get<enums::Monitor_Properties::Resolution>() };
	auto refresh_rate{ parameters.Get<enums::Monitor_Properties::Refresh_Rate>() };
	glfwSetWindowMonitor(underlying_window, underlying_monitor, 0, 0, res_x, res_y, refresh_rate);
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Monitor>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Monitor>::Get(Window& window)
{
	return glfwGetWindowMonitor(window.Get<enums::Window_Properties::Underlying_Window>());
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Iconify>::Set(Window& window, Set_Parameters&& parameters)
{
	if (parameters == true)
	{
		glfwIconifyWindow(window.Get<enums::Window_Properties::Underlying_Window>());
	}
	else if(glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_ICONIFIED) != 0)
	{
		glfwRestoreWindow(window.Get<enums::Window_Properties::Underlying_Window>());
	}
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Iconify>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Iconify>::Get(Window& window)
{
	return glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_ICONIFIED);
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Maximize>::Set(Window& window, Set_Parameters&& parameters)
{
	if (parameters == true)
	{
		glfwMaximizeWindow(window.Get<enums::Window_Properties::Underlying_Window>());
	}
	else if (glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_MAXIMIZED) != 0)
	{
		glfwRestoreWindow(window.Get<enums::Window_Properties::Underlying_Window>());
	}
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Maximize>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Maximize>::Get(Window& window)
{
	return glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_MAXIMIZED);
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Visible>::Set(Window& window, Set_Parameters&& parameters)
{
	if (parameters == true)
	{
		glfwShowWindow(window.Get<enums::Window_Properties::Underlying_Window>());
	}
	else
	{
		glfwHideWindow(window.Get<enums::Window_Properties::Underlying_Window>());
	}
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Visible>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Visible>::Get(Window& window)
{
	return glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_VISIBLE);
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Focus>::Set(Window& window, Set_Parameters&& parameters)
{
	if (parameters == true)
	{
		glfwFocusWindow(window.Get<enums::Window_Properties::Underlying_Window>());
	}
	else if (glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_FOCUSED) == 1)
	{
		glfwFocusWindow(window.s_hidden_window);
	}
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Focus>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Focus>::Get(Window& window)
{
	return glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_FOCUSED);
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Request_Attention>::Set(Window& window)
{
	glfwRequestWindowAttention(window.Get<enums::Window_Properties::Underlying_Window>());
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Opacity>::Set(Window& window, Set_Parameters&& parameters)
{
	glfwSetWindowOpacity(window.Get<enums::Window_Properties::Underlying_Window>(), parameters);
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Opacity>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Opacity>::Get(Window& window)
{
	return glfwGetWindowOpacity(window.Get<enums::Window_Properties::Underlying_Window>());
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Swap_Interval>::Set(Window& window, Set_Parameters&& parameters)
{
	window.m_swap_interval = parameters;
	if (window.m_window == glfwGetCurrentContext())
	{
		glfwSwapInterval(window.m_swap_interval);
	}
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Swap_Interval>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Swap_Interval>::Get(Window& window)
{
	return window.m_swap_interval;
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Resizable>::Set(Window& window, Set_Parameters&& parameters)
{
	glfwSetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_RESIZABLE, parameters);
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Resizable>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Resizable>::Get(Window& window)
{
	return glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_RESIZABLE);
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Decorated>::Set(Window& window, Set_Parameters&& parameters)
{
	glfwSetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_DECORATED, parameters);
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Decorated>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Decorated>::Get(Window& window)
{
	return glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_DECORATED);
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Auto_Iconify>::Set(Window& window, Set_Parameters&& parameters)
{
	glfwSetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_AUTO_ICONIFY, parameters);
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Auto_Iconify>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Auto_Iconify>::Get(Window& window)
{
	return glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_AUTO_ICONIFY);
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Floating>::Set(Window& window, Set_Parameters&& parameters)
{
	glfwSetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_FLOATING, parameters);
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Floating>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Floating>::Get(Window& window)
{
	return glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_FLOATING);
}

void tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Focus_On_Show>::Set(Window& window, Set_Parameters&& parameters)
{
	glfwSetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_FOCUS_ON_SHOW, parameters);
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Focus_On_Show>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Focus_On_Show>::Get(Window& window)
{
	return glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_FOCUS_ON_SHOW);
}

tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Transparent_Framebuffer>::Get_Parameters tilia::windowing::properties::Window_Property<tilia::enums::Window_Properties::Transparent_Framebuffer>::Get(Window& window)
{
	return glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_TRANSPARENT_FRAMEBUFFER);
}

tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::Client_API>::Get_Parameters tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::Client_API>::Get(Window& window)
{
	return static_cast<enums::Client_API>(glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_CLIENT_API));
}

tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::Creation_API>::Get_Parameters tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::Creation_API>::Get(Window& window)
{
	return static_cast<enums::Context_Creation_API>(glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_CONTEXT_CREATION_API));
}

tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::Version>::Get_Parameters tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::Version>::Get(Window& window)
{
	const auto glfw_window{ window.Get<enums::Window_Properties::Underlying_Window>() };
	return { glfwGetWindowAttrib(glfw_window, GLFW_CONTEXT_VERSION_MAJOR), 
			 glfwGetWindowAttrib(glfw_window, GLFW_CONTEXT_VERSION_MINOR), 
			 glfwGetWindowAttrib(glfw_window, GLFW_CONTEXT_REVISION) };
}

tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::OpenGL_Forward_Compatible>::Get_Parameters tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::OpenGL_Forward_Compatible>::Get(Window& window)
{
	return glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_OPENGL_FORWARD_COMPAT);
}

tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::OpenGL_Debug_Context>::Get_Parameters tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::OpenGL_Debug_Context>::Get(Window& window)
{
	return glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_OPENGL_DEBUG_CONTEXT);
}

tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::OpenGL_Profile>::Get_Parameters tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::OpenGL_Profile>::Get(Window& window)
{
	return static_cast<enums::OpenGL_Profile>(glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_OPENGL_PROFILE));
}

tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::Release_Behavior>::Get_Parameters tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::Release_Behavior>::Get(Window& window)
{
	return static_cast<enums::Context_Release_Behavior>(glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_CONTEXT_RELEASE_BEHAVIOR));
}

tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::No_Error>::Get_Parameters tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::No_Error>::Get(Window& window)
{
	return glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_CONTEXT_NO_ERROR);
}

tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::Robustness>::Get_Parameters tilia::windowing::properties::context::Context_Property<tilia::enums::Context_Properties::Robustness>::Get(Window& window)
{
	return static_cast<enums::Context_Robustness>(glfwGetWindowAttrib(window.Get<enums::Window_Properties::Underlying_Window>(), GLFW_CONTEXT_ROBUSTNESS));
}
