// Vendor
#include "vendor/glad/KHR_Debug_openGL_3_3/include/glad/glad.h"
#include "vendor/glfw/include/GLFW/glfw3.h"

// Tilia
#include "Window.hpp"
#include TILIA_LOGGING_INCLUDE

std::unordered_map<tilia::utils::GLFWwindow*, tilia::Window&> tilia::Window::s_windows{ };

void tilia::Window::Set_Window_Hint(const enums::Window_Hints& hint_type, 
	const std::int32_t& hint_value)
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

void tilia::Window::Init(const std::int32_t& width, const std::int32_t& height, 
	const std::string& title, void* monitor, void* share)
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
}

void tilia::Window::Make_Context_Current() const
{
	glfwMakeContextCurrent(m_window);
}

void tilia::Window::Swap_Buffers() const
{
	glfwSwapBuffers(m_window);
}

//template<>
//void tilia::Window::Set<tilia::enums::Window_Properties::Should_Close>(bool should_close)
//{
//	glfwSetWindowShouldClose(m_window, should_close);
//}
//
//template<>
//void tilia::Window::Set<tilia::enums::Window_Properties::Size>(std::int32_t width, 
//	std::int32_t height)
//{
//	glfwSetWindowSize(m_window, width, height);
//}
//
//template<>
//void tilia::Window::Set<tilia::enums::Window_Properties::Size>(std::int32_t min_width, 
//	std::int32_t min_height, std::int32_t max_width, std::int32_t max_height)
//{
//	glfwSetWindowSizeLimits(m_window, min_width, min_height, max_width, max_height);
//}
//
//template<>
//void tilia::Window::Set<tilia::enums::Window_Properties::Aspect_Ratio>(std::int32_t numer,
//	std::int32_t denom)
//{
//	glfwSetWindowAspectRatio(m_window, numer, denom);
//}
//
//template<>
//void tilia::Window::Set<tilia::enums::Window_Properties::Position>(std::int32_t x_pos,
//	std::int32_t y_pos)
//{
//	glfwSetWindowPos(m_window, x_pos, y_pos);
//}
//
//template<>
//void tilia::Window::Set<tilia::enums::Window_Properties::Title>(std::string title)
//{
//	glfwSetWindowTitle(m_window, title.c_str());
//}
//
//template<>
//void tilia::Window::Set<tilia::enums::Window_Properties::Icon>()
//{
//	tilia::log::Logger::Instance().Output("Can't set icon yet\n");
//}
//
//template<>
//void tilia::Window::Set<tilia::enums::Window_Properties::Monitor>()
//{
//	tilia::log::Logger::Instance().Output("Can't set monitor yet\n");
//}
//
//template<>
//void tilia::Window::Set<tilia::enums::Window_Properties::Iconify>(bool iconify)
//{
//	if (iconify)
//		glfwIconifyWindow(m_window);
//	else
//		glfwRestoreWindow(m_window);
//}
//
//template<>
//void tilia::Window::Set<tilia::enums::Window_Properties::Maximize>(bool maximize)
//{
//	if (maximize)
//		glfwMaximizeWindow(m_window);
//	else
//		glfwRestoreWindow(m_window);
//}
//
//template<>
//void tilia::Window::Set<tilia::enums::Window_Properties::Visible>(bool visible)
//{
//	tilia::log::Logger::Instance().Output("visible\n");
//	if (visible)
//		glfwShowWindow(m_window);
//	else
//		glfwHideWindow(m_window);
//}
//
//template<>
//void tilia::Window::Set<tilia::enums::Window_Properties::Visible>()
//{
//	glfwFocusWindow(m_window);
//}



void tilia::Window::Set_Callback(const enums::Window_Callbacks& type, callback_ptr callback)
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

static inline void Set_Should_Close(tilia::utils::GLFWwindow* window, void* property)
{
	auto& should_close{ *static_cast<tilia::properties::Should_Close_Parameters*>(property) };
	glfwSetWindowShouldClose(window, std::get<0>(should_close));
}

static inline void Set_Size(tilia::utils::GLFWwindow* window, void* property)
{
	auto& size{ *static_cast<tilia::properties::Size_Parameters*>(property) };
	glfwSetWindowSize(window, std::get<0>(size), std::get<1>(size));
}

static inline void Set_Size_Limits(tilia::utils::GLFWwindow* window, void* property)
{
	auto& size_limits{ *static_cast<tilia::properties::Size_Limits_Parameters*>(property) };
	glfwSetWindowSizeLimits(window, std::get<0>(size_limits), std::get<1>(size_limits),
		std::get<2>(size_limits), std::get<3>(size_limits));
}

static inline void Set_Size(tilia::utils::GLFWwindow* window, void* property)
{
	auto& aspect_ratio{ *static_cast<tilia::properties::Aspect_Ratio_Parameters*>(property) };
	glfwSetWindowAspectRatio(window, std::get<0>(aspect_ratio), std::get<1>(aspect_ratio));
}

static inline void Set_Position(tilia::utils::GLFWwindow* window, void* property)
{
	auto& position{ *static_cast<tilia::properties::Position_Parameters*>(property) };
	glfwSetWindowPos(window, std::get<0>(position), std::get<1>(position));
}

static inline void Set_Title(tilia::utils::GLFWwindow* window, void* property)
{
	auto& title{ *static_cast<tilia::properties::Title_Parameters*>(property) };
	glfwSetWindowTitle(window, std::get<0>(title).c_str());
}

void tilia::Window::Set_Property(const enums::Window_Properties& type, property_ptr property)
{
	switch (type)
	{
	case enums::Window_Properties::Should_Close:
		Set_Should_Close(m_window, property);
		break;
	case enums::Window_Properties::Size:
		Set_Size(m_window, property);
		break;
	}
}

static inline void Get_Should_Close(tilia::utils::GLFWwindow* window, void* property)
{
	auto& should_close{ *static_cast<tilia::properties::Should_Close_Parameters*>(property) };
	std::get<0>(should_close) = glfwWindowShouldClose(window);
}

static inline void Get_Size(tilia::utils::GLFWwindow* window, void* property)
{
	auto& size{ *static_cast<tilia::properties::Size_Parameters*>(property) };
	glfwGetWindowSize(window, &std::get<0>(size), &std::get<1>(size));
}

static inline void Get_Frame_Size(tilia::utils::GLFWwindow* window, void* property)
{
	auto& frame_size{ *static_cast<tilia::properties::Frame_Size_Parameters*>(property) };
	glfwGetWindowFrameSize(window, &std::get<0>(frame_size), &std::get<1>(frame_size),
		&std::get<2>(frame_size), &std::get<3>(frame_size));
}

static inline void Get_Framebuffer_Size(tilia::utils::GLFWwindow* window, void* property)
{
	auto& framebuffer_size{ 
		*static_cast<tilia::properties::Framebuffer_Size_Parameters*>(property) };
	glfwGetFramebufferSize(window, &std::get<0>(framebuffer_size), &std::get<1>(framebuffer_size));
}

static inline void Get_Content_Scale(tilia::utils::GLFWwindow* window, void* property)
{
	auto& content_scale_size{
		*static_cast<tilia::properties::Content_Scale_Parameters*>(property) };
	glfwGetWindowContentScale(window, 
		&std::get<0>(content_scale_size), &std::get<1>(content_scale_size));
}

static inline void Get_Position(tilia::utils::GLFWwindow* window, void* property)
{
	auto& position{ *static_cast<tilia::properties::Position_Parameters*>(property) };
	glfwGetWindowPos(window, &std::get<0>(position), &std::get<1>(position));
}

void tilia::Window::Get_Property(const enums::Window_Properties& type, void* property)
{
	switch (type)
	{
	case enums::Window_Properties::Should_Close:
		Get_Should_Close(m_window, property);
		break;
	case enums::Window_Properties::Size:
		Get_Size(m_window, property);
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
