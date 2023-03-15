// Vendor
#include "vendor/glad/KHR_Debug_openGL_3_3/include/glad/glad.h"
#include "vendor/glfw/include/GLFW/glfw3.h"

// Tilia
#include "Window.hpp"
#include TILIA_LOGGING_INCLUDE

std::unordered_map<tilia::utils::GLFWwindow*, tilia::Window&> tilia::Window::s_windows{ };

void tilia::Window::Set_Window_Hint(enums::Window_Hints hint_type, 
	std::int32_t hint_value)
{
	if (hint_type == enums::Window_Hints::Reset)
	{
		glfwDefaultWindowHints();
		return;
	}
	glfwWindowHint(*hint_type, hint_value);
}

void tilia::Window::Init()
{
	glfwInit();
}

void tilia::Window::Terminate()
{
	glfwTerminate();
}

tilia::Window::~Window()
{
	if (m_window != nullptr)
	{
		glfwDestroyWindow(m_window);
		s_windows.erase(m_window);
		m_window = nullptr;
	}
	if (s_windows.size() == 0)
		glfwTerminate();
}

void tilia::Window::Init(std::int32_t width, std::int32_t height, 
	std::string title, void* monitor, void* share)
{

	m_window = glfwCreateWindow(width, height, title.c_str(),
		static_cast<GLFWmonitor*>(monitor), static_cast<GLFWwindow*>(share));

	s_windows.insert({ m_window, *this });

	Set_Callback(enums::Window_Callbacks::Position, Position_Func);

	Set_Callback(enums::Window_Callbacks::Size, Size_Func);

	Set_Callback(enums::Window_Callbacks::Close, Close_Func);

	Set_Callback(enums::Window_Callbacks::Refresh, Refresh_Func);

	Set_Callback(enums::Window_Callbacks::Focus, Focus_Func);

	Set_Callback(enums::Window_Callbacks::Iconify, Inconify_Func);

	Set_Callback(enums::Window_Callbacks::Maximize, Maximize_Func);

	Set_Callback(enums::Window_Callbacks::Framebuffer_Size, Framebuffer_Size_Func);

	Set_Callback(enums::Window_Callbacks::Content_Scale, Content_Scale_Func);

}

void tilia::Window::Destroy()
{
	glfwDestroyWindow(m_window);
	s_windows.erase(m_window);
	m_window = nullptr;
	if (s_windows.size() == 0)
		glfwTerminate();
	m_swap_interval = 1;
}

void tilia::Window::Make_Context_Current() const
{
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(m_swap_interval);
}

void tilia::Window::Swap_Buffers() const
{
	glfwSwapBuffers(m_window);
}

void tilia::Window::Set_Callback(enums::Window_Callbacks type, callback_ptr callback)
{
	switch (type)
	{
	case enums::Window_Callbacks::Position:
		glfwSetWindowPosCallback(m_window, 
			static_cast<GLFWwindowposfun>(callback));
		break;
	case enums::Window_Callbacks::Size:
		glfwSetWindowSizeCallback(m_window, 
			static_cast<GLFWwindowsizefun>(callback));
		break;
	case enums::Window_Callbacks::Close:
		glfwSetWindowCloseCallback(m_window, 
			static_cast<GLFWwindowclosefun>(callback));
		break;
	case enums::Window_Callbacks::Refresh:
		glfwSetWindowRefreshCallback(m_window, 
			static_cast<GLFWwindowrefreshfun>(callback));
		break;
	case enums::Window_Callbacks::Focus:
		glfwSetWindowFocusCallback(m_window, 
			static_cast<GLFWwindowfocusfun>(callback));
		break;
	case enums::Window_Callbacks::Iconify:
		glfwSetWindowIconifyCallback(m_window, 
			static_cast<GLFWwindowiconifyfun>(callback));
		break;
	case enums::Window_Callbacks::Maximize:
		glfwSetWindowMaximizeCallback(m_window, 
			static_cast<GLFWwindowmaximizefun>(callback));
		break;
	case enums::Window_Callbacks::Framebuffer_Size:
		glfwSetFramebufferSizeCallback(m_window, 
			static_cast<GLFWframebuffersizefun>(callback));
		break;
	case enums::Window_Callbacks::Content_Scale:
		glfwSetWindowContentScaleCallback(m_window, 
			static_cast<GLFWwindowcontentscalefun>(callback));
		break;
	}
}

void tilia::Window::Position_Func(utils::GLFWwindow* window, std::int32_t x_pos, std::int32_t y_pos)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Window_Callbacks::Position>(instance.m_callbacks))
	{
		func(window, x_pos, y_pos);
	}
}

void tilia::Window::Size_Func(utils::GLFWwindow* window, std::int32_t width, std::int32_t height)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Window_Callbacks::Size>(instance.m_callbacks))
	{
		func(window, width, height);
	}
}

void tilia::Window::Close_Func(utils::GLFWwindow* window)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Window_Callbacks::Close>(instance.m_callbacks))
	{
		func(window);
	}
}

void tilia::Window::Refresh_Func(utils::GLFWwindow* window)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Window_Callbacks::Refresh>(instance.m_callbacks))
	{
		func(window);
	}
}

void tilia::Window::Focus_Func(utils::GLFWwindow* window, std::int32_t focused)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Window_Callbacks::Focus>(instance.m_callbacks))
	{
		func(window, focused);
	}
}

void tilia::Window::Inconify_Func(utils::GLFWwindow* window, std::int32_t iconified)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Window_Callbacks::Iconify>(instance.m_callbacks))
	{
		func(window, iconified);
	}
}

void tilia::Window::Maximize_Func(utils::GLFWwindow* window, std::int32_t maximized)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Window_Callbacks::Maximize>(instance.m_callbacks))
	{
		func(window, maximized);
	}
}

void tilia::Window::Framebuffer_Size_Func(utils::GLFWwindow* window, std::int32_t width, std::int32_t height)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Window_Callbacks::Framebuffer_Size>(instance.m_callbacks))
	{
		func(window, width, height);
	}
}

void tilia::Window::Content_Scale_Func(utils::GLFWwindow* window, float x_scale, float y_scale)
{
	Window& instance{ s_windows.at(window) };
	for (auto& func : std::get<*enums::Window_Callbacks::Content_Scale>(instance.m_callbacks))
	{
		func(window, x_scale, y_scale);
	}
}
