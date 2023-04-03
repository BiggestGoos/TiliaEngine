// Vendor
#include "vendor/glad/KHR_Debug_openGL_3_3/include/glad/glad.h"
#include "vendor/glfw/include/GLFW/glfw3.h"

// Tilia
#include "Window.hpp"
#include TILIA_LOGGING_INCLUDE

GLFWwindow* tilia::windowing::Window::s_hidden_window{ };
std::unordered_map<tilia::windowing::GLFWwindow*, tilia::windowing::Window&> tilia::windowing::Window::s_windows{ };

void tilia::windowing::Window::Init()
{
	glfwInit();

	monitoring::Monitor::Init();

	// Create a hidden window which can be used to set focus to

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	s_hidden_window = glfwCreateWindow(1, 1, "", nullptr, nullptr);

	glfwDefaultWindowHints();

}

void tilia::windowing::Window::Terminate()
{
	glfwTerminate();
}

tilia::windowing::Window::~Window()
{
	if (m_window != nullptr)
	{
		glfwDestroyWindow(m_window);
		s_windows.erase(m_window);
		m_window = nullptr;
	}
}

void tilia::windowing::Window::Init(std::int32_t width, std::int32_t height,
	std::string title, void* monitor, void* share)
{

	m_window = glfwCreateWindow(width, height, title.c_str(),
		static_cast<GLFWmonitor*>(monitor), static_cast<GLFWwindow*>(share));

	if (title != "")
		m_title = title;

	s_windows.insert({ m_window, *this });

	Set_Callback(enums::Callbacks::Position, Position_Func);

	Set_Callback(enums::Callbacks::Size, Size_Func);

	Set_Callback(enums::Callbacks::Close, Close_Func);

	Set_Callback(enums::Callbacks::Refresh, Refresh_Func);

	Set_Callback(enums::Callbacks::Focus, Focus_Func);

	Set_Callback(enums::Callbacks::Iconify, Inconify_Func);

	Set_Callback(enums::Callbacks::Maximize, Maximize_Func);

	Set_Callback(enums::Callbacks::Framebuffer_Size, Framebuffer_Size_Func);

	Set_Callback(enums::Callbacks::Content_Scale, Content_Scale_Func);

}

void tilia::windowing::Window::Destroy()
{
	glfwDestroyWindow(m_window);
	s_windows.erase(m_window);
	m_window = nullptr;
	m_title = "GLFW";
	m_swap_interval = 1;
}

void tilia::windowing::Window::Make_Context_Current() const
{
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(m_swap_interval);
}

void tilia::windowing::Window::Swap_Buffers() const
{
	glfwSwapBuffers(m_window);
}

void tilia::windowing::Window::Set_Callback(enums::Callbacks type, callback_ptr callback)
{
	switch (type)
	{
	case enums::Callbacks::Position:
		glfwSetWindowPosCallback(m_window, 
			static_cast<GLFWwindowposfun>(callback));
		break;
	case enums::Callbacks::Size:
		glfwSetWindowSizeCallback(m_window, 
			static_cast<GLFWwindowsizefun>(callback));
		break;
	case enums::Callbacks::Close:
		glfwSetWindowCloseCallback(m_window, 
			static_cast<GLFWwindowclosefun>(callback));
		break;
	case enums::Callbacks::Refresh:
		glfwSetWindowRefreshCallback(m_window, 
			static_cast<GLFWwindowrefreshfun>(callback));
		break;
	case enums::Callbacks::Focus:
		glfwSetWindowFocusCallback(m_window, 
			static_cast<GLFWwindowfocusfun>(callback));
		break;
	case enums::Callbacks::Iconify:
		glfwSetWindowIconifyCallback(m_window, 
			static_cast<GLFWwindowiconifyfun>(callback));
		break;
	case enums::Callbacks::Maximize:
		glfwSetWindowMaximizeCallback(m_window, 
			static_cast<GLFWwindowmaximizefun>(callback));
		break;
	case enums::Callbacks::Framebuffer_Size:
		glfwSetFramebufferSizeCallback(m_window, 
			static_cast<GLFWframebuffersizefun>(callback));
		break;
	case enums::Callbacks::Content_Scale:
		glfwSetWindowContentScaleCallback(m_window, 
			static_cast<GLFWwindowcontentscalefun>(callback));
		break;
	}
}

void tilia::windowing::Window::Position_Func(GLFWwindow* window, std::int32_t x_pos, std::int32_t y_pos)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Callbacks::Position>(instance.m_callbacks))
	{
		func.function(window, x_pos, y_pos);
	}
}

void tilia::windowing::Window::Size_Func(GLFWwindow* window, std::int32_t width, std::int32_t height)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Callbacks::Size>(instance.m_callbacks))
	{
		func.function(window, width, height);
	}
}

void tilia::windowing::Window::Close_Func(GLFWwindow* window)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Callbacks::Close>(instance.m_callbacks))
	{
		func.function(window);
	}
}

void tilia::windowing::Window::Refresh_Func(GLFWwindow* window)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Callbacks::Refresh>(instance.m_callbacks))
	{
		func.function(window);
	}
}

void tilia::windowing::Window::Focus_Func(GLFWwindow* window, std::int32_t focused)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Callbacks::Focus>(instance.m_callbacks))
	{
		func.function(window, focused);
	}
}

void tilia::windowing::Window::Inconify_Func(GLFWwindow* window, std::int32_t iconified)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Callbacks::Iconify>(instance.m_callbacks))
	{
		func.function(window, iconified);
	}
}

void tilia::windowing::Window::Maximize_Func(GLFWwindow* window, std::int32_t maximized)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Callbacks::Maximize>(instance.m_callbacks))
	{
		func.function(window, maximized);
	}
}

void tilia::windowing::Window::Framebuffer_Size_Func(GLFWwindow* window, std::int32_t width, std::int32_t height)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Callbacks::Framebuffer_Size>(instance.m_callbacks))
	{
		func.function(window, width, height);
	}
}

void tilia::windowing::Window::Content_Scale_Func(GLFWwindow* window, float x_scale, float y_scale)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Callbacks::Content_Scale>(instance.m_callbacks))
	{
		func.function(window, x_scale, y_scale);
	}
}
