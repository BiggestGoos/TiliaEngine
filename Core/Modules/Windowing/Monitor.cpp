// Vendor
#include "vendor/glfw/include/GLFW/glfw3.h"

// Tilia
#include "Monitor.hpp"

tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::Resolution>::
Get_Parameters tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::
	Resolution>::Get(Monitor& monitor)
{
	const GLFWvidmode* video_mode{ 
		glfwGetVideoMode(monitor.Get<enums::Monitor_Properties::Underlying_Monitor>()) };
	return { video_mode->width, video_mode->height };
}

tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::Bit_Depths>::
Get_Parameters tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::
	Bit_Depths>::Get(Monitor& monitor)
{
	const GLFWvidmode* video_mode{ 
		glfwGetVideoMode(monitor.Get<enums::Monitor_Properties::Underlying_Monitor>()) };
	return { video_mode->redBits, video_mode->greenBits, video_mode->blueBits };
}

tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::Refresh_Rate>::
Get_Parameters tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::
	Refresh_Rate>::Get(Monitor& monitor)
{
	const GLFWvidmode* video_mode{ 
		glfwGetVideoMode(monitor.Get<enums::Monitor_Properties::Underlying_Monitor>()) };
	return video_mode->refreshRate;
}

tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::
	Supported_Resolutions>::Get_Parameters tilia::monitoring::properties::Monitor_Property<tilia::
	enums::Monitor_Properties::Supported_Resolutions>::Get(Monitor& monitor)
{
	Get_Parameters ret_val{};
	std::int32_t count{};
	const GLFWvidmode* video_modes{ 
		glfwGetVideoModes(monitor.Get<enums::Monitor_Properties::Underlying_Monitor>(), &count) };
	for (std::int32_t i{ 0 }; i < count; ++i)
	{
		ret_val.insert({ video_modes[i].width, video_modes[i].height });
	}
	return ret_val;
}

tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::
	Supported_Bit_Depths>::Get_Parameters tilia::monitoring::properties::Monitor_Property<tilia::
	enums::Monitor_Properties::Supported_Bit_Depths>::Get(Monitor& monitor, 
		Get_Arguments&& get_arguments)
{
	std::int32_t count{};
	const GLFWvidmode* video_modes{ 
		glfwGetVideoModes(monitor.Get<enums::Monitor_Properties::Underlying_Monitor>(), &count) };
	Get_Parameters ret_val{};
	for (std::int32_t i{ 0 }; i < count; ++i)
	{
		if (video_modes[i].width == get_arguments.first && 
			video_modes[i].height == get_arguments.second)
		{
			ret_val.push_back({ video_modes[i].redBits, 
				video_modes[i].greenBits, video_modes[i].blueBits });
		}
	}
	return ret_val;
}

tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::
	Supported_Refresh_Rates>::Get_Parameters tilia::monitoring::properties::Monitor_Property<
	tilia::enums::Monitor_Properties::Supported_Refresh_Rates>::Get(Monitor& monitor, 
		Get_Arguments&& get_arguments)
{
	std::int32_t count{};
	const GLFWvidmode* video_modes{ 
		glfwGetVideoModes(monitor.Get<enums::Monitor_Properties::Underlying_Monitor>(), &count) };
	Get_Parameters ret_val{};
	for (std::int32_t i{ 0 }; i < count; ++i)
	{
		if (video_modes[i].width == get_arguments.first && 
			video_modes[i].height == get_arguments.second)
		{
			ret_val.push_back(video_modes[i].refreshRate);
		}
	}
	return ret_val;
}

tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::Physical_Size>::
Get_Parameters tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::
	Physical_Size>::Get(Monitor& monitor)
{
	Get_Parameters ret_val{};
	glfwGetMonitorPhysicalSize(monitor.Get<enums::Monitor_Properties::Underlying_Monitor>(), 
		&ret_val.first, &ret_val.second);
	return ret_val;
}

tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::Content_Scale>::
Get_Parameters tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::
	Content_Scale>::Get(Monitor& monitor)
{
	Get_Parameters ret_val{};
	glfwGetMonitorContentScale(monitor.Get<enums::Monitor_Properties::Underlying_Monitor>(), 
		&ret_val.first, &ret_val.second);
	return ret_val;
}

tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::Virtual_Position>
::Get_Parameters tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::
	Virtual_Position>::Get(Monitor& monitor)
{
	Get_Parameters ret_val{};
	glfwGetMonitorPos(monitor.Get<enums::Monitor_Properties::Underlying_Monitor>(), 
		&ret_val.first, &ret_val.second);
	return ret_val;
}

tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::Workarea>::
Get_Parameters tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::
	Workarea>::Get(Monitor& monitor)
{
	Get_Parameters ret_val{};
	glfwGetMonitorWorkarea(monitor.Get<enums::Monitor_Properties::Underlying_Monitor>(), 
		&std::get<0>(ret_val), &std::get<1>(ret_val), 
		&std::get<2>(ret_val), &std::get<3>(ret_val));
	return ret_val;
}

tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::Name>::
Get_Parameters tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::
	Name>::Get(Monitor& monitor)
{
	return glfwGetMonitorName(monitor.Get<enums::Monitor_Properties::Underlying_Monitor>());
}

void tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::Gamma_Ramp>
::Set(Monitor& monitor, Set_Parameters&& parameters)
{
	GLFWgammaramp gamma_ramp{};
	gamma_ramp.size = static_cast<decltype(GLFWgammaramp::size)>(parameters.size());
	std::vector<std::uint16_t> 
		red_bits(gamma_ramp.size), 
		green_bits(gamma_ramp.size), 
		blue_bits(gamma_ramp.size);
	for (std::size_t i{ 0 }; i < gamma_ramp.size; ++i)
	{
		const auto& [red, green, blue] = parameters[i];
		red_bits[i] = red;
		green_bits[i] = green;
		blue_bits[i] = blue;
	}
	gamma_ramp.red = red_bits.data();
	gamma_ramp.green = red_bits.data();
	gamma_ramp.blue = red_bits.data();
	glfwSetGammaRamp(monitor.Get<enums::Monitor_Properties::Underlying_Monitor>(), &gamma_ramp);
}

tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::Gamma_Ramp>::
Get_Parameters tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::
	Gamma_Ramp>::Get(Monitor& monitor)
{
	const GLFWgammaramp* gamma_ramp{ 
		glfwGetGammaRamp(monitor.Get<enums::Monitor_Properties::Underlying_Monitor>()) };
	Get_Parameters ret_val(gamma_ramp->size);
	for (std::size_t i{ 0 }; i < gamma_ramp->size; ++i)
	{
		ret_val[i] = { gamma_ramp->red[i], gamma_ramp->green[i], gamma_ramp->blue[i] };
	}
	return ret_val;
}

void tilia::monitoring::properties::Monitor_Property<tilia::enums::Monitor_Properties::Gamma>::
Set(Monitor& monitor, Set_Parameters&& parameters)
{
	glfwSetGamma(monitor.Get<enums::Monitor_Properties::Underlying_Monitor>(), parameters);
}

/* Monitor class starts here */

std::set<tilia::monitoring::Monitor> tilia::monitoring::Monitor::s_monitors{ };
std::vector<tilia::monitoring::callbacks::Monitor_Func> tilia::monitoring::Monitor::s_callbacks{ };

bool tilia::monitoring::Monitor::Is_Connected() const
{
	return (s_monitors.find(m_monitor) != s_monitors.end());
}

std::vector<tilia::monitoring::Monitor> tilia::monitoring::Monitor::Get_Monitors()
{
	std::vector<Monitor> ret_val{};
	std::int32_t monitor_count{};
	GLFWmonitor** monitors{ glfwGetMonitors(&monitor_count) };
	for (std::size_t i{ 0 }; i < monitor_count; ++i)
	{
		ret_val.push_back(monitors[i]);
	}
	return ret_val;
}

template<typename Func>
static bool is_same_function(Func& f1, Func& f2) {
	return f1.target_type() == f2.target_type() &&
		*reinterpret_cast<void**>(&f1) == *reinterpret_cast<void**>(&f2);
}

void tilia::monitoring::Monitor::Remove_Callback(callbacks::Monitor_Func callback)
{
	const auto callback_count{ s_callbacks.size() };
	std::size_t i{ 0 };
	for (; i < callback_count; ++i)
	{
		if (is_same_function(callback.function, s_callbacks[i].function))
			break;
	}
	if (i < callback_count)
		s_callbacks.erase(s_callbacks.begin() + i);
}

void tilia::monitoring::Monitor::Init()
{

	glfwSetMonitorCallback(Monitor::Monitor_Func);

	std::int32_t monitor_count{};
	GLFWmonitor** monitors{ glfwGetMonitors(&monitor_count) };

	for (std::int32_t i{ 0 }; i < monitor_count; ++i)
	{
		s_monitors.insert(monitors[i]);
	}

}

void tilia::monitoring::Monitor::Monitor_Func(GLFWmonitor* monitor, std::int32_t event)
{
	if (event == GLFW_CONNECTED)
	{
		s_monitors.insert(monitor);
	}
	else if (event == GLFW_DISCONNECTED)
	{
		s_monitors.erase(monitor);
	}
	for (auto& callback : s_callbacks)
	{
		callback.function(monitor, event);
	}
}

#if TILIA_UNIT_TESTS == 1

// Standard
#include <sstream>
#include <iostream>

static std::size_t callback_number{};
static GLFWmonitor* callback_monitor{};
static std::int32_t callback_event{};

void test_monitor_callback_0(GLFWmonitor* monitor, std::int32_t event)
{
	callback_number = 0;
	callback_monitor = monitor;
	callback_event = event;
}

struct test_monitor_callback_1_struct
{
	void test_monitor_callback_1(GLFWmonitor* monitor, std::int32_t event)
	{
		callback_number = 1;
		callback_monitor = monitor;
		callback_event = event;
	}
};

static auto test_monitor_callback_2 {
	[](GLFWmonitor* monitor, std::int32_t event)
	{
	callback_number = 2;
	callback_monitor = monitor;
	callback_event = event;	}
};

// Vendor
#include "vendor/Catch2/Catch2.hpp"

void tilia::monitoring::Monitor::Test()
{

	Monitor::Init();

	// Test that first gotten monitor by 'Get_Monitors' is primary monitor

	GLFWmonitor* base_primary_monitor_0{ glfwGetPrimaryMonitor() };

	Monitor primary_monitor_0{ Monitor::Get_Monitors()[0] };

	REQUIRE(primary_monitor_0.Get<enums::Monitor_Properties::Underlying_Monitor>() 
		== base_primary_monitor_0);

	// Test that the monitor is recognized as connected

	REQUIRE(primary_monitor_0.Is_Connected());

	// Test callback with regular free function

	primary_monitor_0.Add_Callback(callbacks::Monitor_Func{ test_monitor_callback_0 });

	primary_monitor_0.Monitor_Func(base_primary_monitor_0, 123);

	REQUIRE(callback_number == 0);
	REQUIRE(callback_monitor == base_primary_monitor_0);
	REQUIRE(callback_event == 123);

	primary_monitor_0.Remove_Callback(callbacks::Monitor_Func{ test_monitor_callback_0 });

	// Test callback with member function

	test_monitor_callback_1_struct test_monitor_callback_1_object_0{};

	auto member_func_callback_0{ 
		std::bind(&test_monitor_callback_1_struct::test_monitor_callback_1, 
		&test_monitor_callback_1_object_0, std::placeholders::_1, std::placeholders::_2) };

	primary_monitor_0.Add_Callback(callbacks::Monitor_Func{ member_func_callback_0 });

	primary_monitor_0.Monitor_Func(nullptr, 245);

	REQUIRE(callback_number == 1);
	REQUIRE(callback_monitor == nullptr);
	REQUIRE(callback_event == 245);

	primary_monitor_0.Remove_Callback(callbacks::Monitor_Func{ member_func_callback_0 });

	// Test callback with lambda function

	primary_monitor_0.Add_Callback(callbacks::Monitor_Func{ test_monitor_callback_2 });

	primary_monitor_0.Monitor_Func(base_primary_monitor_0, 678);

	REQUIRE(callback_number == 2);
	REQUIRE(callback_monitor == base_primary_monitor_0);
	REQUIRE(callback_event == 678);

	primary_monitor_0.Remove_Callback(callbacks::Monitor_Func{ test_monitor_callback_2 });

	// Test for video mode of monitor

	const GLFWvidmode* video_mode_0{ glfwGetVideoMode(base_primary_monitor_0) };

		// Resolution

	auto [res_x_0, res_y_0] { primary_monitor_0.Get<enums::Monitor_Properties::Resolution>() };

	REQUIRE(res_x_0 == video_mode_0->width);
	REQUIRE(res_y_0 == video_mode_0->height);

		// Bit depths

	auto [red_bits_0, green_bits_0, blue_bits_0]{ 
		primary_monitor_0.Get<enums::Monitor_Properties::Bit_Depths>() };

	REQUIRE(red_bits_0 == video_mode_0->redBits);
	REQUIRE(green_bits_0 == video_mode_0->blueBits);
	REQUIRE(blue_bits_0 == video_mode_0->greenBits);

		// Refresh rate

	auto refresh_rate_0{ primary_monitor_0.Get<enums::Monitor_Properties::Refresh_Rate>() };

	REQUIRE(refresh_rate_0 == video_mode_0->refreshRate);

	// Test for supported video modes of monitor

		// Resolutions
		
	auto supported_resolutions_0{ 
		primary_monitor_0.Get<enums::Monitor_Properties::Supported_Resolutions>() };

	REQUIRE(supported_resolutions_0.size() > 0);

	for (const auto& resolution_0 : supported_resolutions_0)
	{

		// Bit depths

		auto supported_bit_depths_0{ 
			primary_monitor_0.Get<enums::Monitor_Properties::Supported_Bit_Depths>(resolution_0) };

		REQUIRE(supported_bit_depths_0.size() > 0);

		// Refresh rates

		auto supported_refresh_rates_0{ 
			primary_monitor_0.Get<enums::Monitor_Properties::Supported_Refresh_Rates>(
				resolution_0) };

		REQUIRE(supported_refresh_rates_0.size() > 0);

	}

	// Test for getting misc values

		// Physical size

	auto [physical_width_0, physical_height_0] { 
		primary_monitor_0.Get<enums::Monitor_Properties::Physical_Size>() };

	std::int32_t physical_width_1{}, physical_height_1{};

	glfwGetMonitorPhysicalSize(base_primary_monitor_0, &physical_width_1, &physical_height_1);

	REQUIRE(physical_width_0 == physical_width_1);
	REQUIRE(physical_height_0 == physical_height_1);

		// Content scale

	auto [content_scale_x_0, content_scale_y_0] {
		primary_monitor_0.Get<enums::Monitor_Properties::Content_Scale>() };

	float content_scale_x_1{}, content_scale_y_1{};

	glfwGetMonitorContentScale(base_primary_monitor_0, &content_scale_x_1, &content_scale_y_1);

	REQUIRE(content_scale_x_0 == content_scale_x_1);
	REQUIRE(content_scale_y_0 == content_scale_y_1);

		// Virtual position

	auto [virtual_pos_x_0, virtual_pos_y_0] {
		primary_monitor_0.Get<enums::Monitor_Properties::Virtual_Position>() };

	std::int32_t virtual_pos_x_1{}, virtual_pos_y_1{};

	glfwGetMonitorPos(base_primary_monitor_0, &virtual_pos_x_1, &virtual_pos_y_1);

	REQUIRE(virtual_pos_x_0 == virtual_pos_x_1);
	REQUIRE(virtual_pos_y_0 == virtual_pos_y_1);

		// Workarea

	auto [work_area_pos_x_0, work_area_pos_y_0, work_area_width_0, work_area_height_0] {
		primary_monitor_0.Get<enums::Monitor_Properties::Workarea>() };

	std::int32_t 
		work_area_pos_x_1{}, work_area_pos_y_1{}, work_area_width_1{}, work_area_height_1{};

	glfwGetMonitorWorkarea(base_primary_monitor_0, &work_area_pos_x_1, &work_area_pos_y_1, 
		&work_area_width_1, &work_area_height_1);

	REQUIRE(work_area_pos_x_0 == work_area_pos_x_1);
	REQUIRE(work_area_pos_y_0 == work_area_pos_y_1);
	REQUIRE(work_area_width_0 == work_area_width_1);
	REQUIRE(work_area_height_0 == work_area_height_1);

		// Name

	auto name_0{ primary_monitor_0.Get<enums::Monitor_Properties::Name>() };

	std::string name_1{ glfwGetMonitorName(base_primary_monitor_0) };

	REQUIRE(name_0 == name_1);

		// Gamma ramp

	// get the gamma ramp from before testing
	auto original_gamma_ramp{ primary_monitor_0.Get<enums::Monitor_Properties::Gamma_Ramp>() };

		// Test getting

	auto gamma_ramp_0{ primary_monitor_0.Get<enums::Monitor_Properties::Gamma_Ramp>() };

	auto compare_gamma_ramps{
		[](const Gamma_Ramp& lhs, const GLFWgammaramp* rhs)
		{
			const auto gamma_ramp_size_0{ rhs->size };
			for (std::size_t i{ 0 }; i < gamma_ramp_size_0; ++i)
			{
				const auto& [red_bits_1, green_bits_1, blue_bits_1] { lhs[i] };
				REQUIRE(red_bits_1 == rhs->red[i]);
				REQUIRE(green_bits_1 == rhs->green[i]);
				REQUIRE(blue_bits_1 == rhs->blue[i]);
			}
		}
	};

	const GLFWgammaramp* gamma_ramp_1{ glfwGetGammaRamp(base_primary_monitor_0) };

	compare_gamma_ramps(gamma_ramp_0, gamma_ramp_1);

		// Test setting

	// generates a gamma ramp
	auto generate_gamma_ramp{
		[](Gamma_Ramp& ramp, float gamma_value)
		{
			const auto gamma_ramp_size_0{ ramp.size() };
			for (std::size_t i{ 0 }; i < gamma_ramp_size_0; ++i) {
				float gamma = static_cast<float>(i) / static_cast<float>(gamma_ramp_size_0 - 1);
				gamma = std::powf(gamma, 1.0f / gamma_value);
				ramp[i] = { static_cast<std::uint16_t>(gamma * 65535.0f),
				static_cast<std::uint16_t>(gamma * 65535.0f),
				static_cast<std::uint16_t>(gamma * 65535.0f) };
			}
		}
	};

	generate_gamma_ramp(gamma_ramp_0, 2.2f);

	primary_monitor_0.Set<enums::Monitor_Properties::Gamma_Ramp>(gamma_ramp_0);

	gamma_ramp_1 = glfwGetGammaRamp(base_primary_monitor_0);

	compare_gamma_ramps(gamma_ramp_0, gamma_ramp_1);

		// Gamma

	generate_gamma_ramp(gamma_ramp_0, 1.0f);

	primary_monitor_0.Set<enums::Monitor_Properties::Gamma>(1.0f);

	gamma_ramp_1 = glfwGetGammaRamp(base_primary_monitor_0);

	compare_gamma_ramps(gamma_ramp_0, gamma_ramp_1);

	// reset gamma ramp to original
	primary_monitor_0.Set<enums::Monitor_Properties::Gamma_Ramp>(original_gamma_ramp);

}

#endif // TILIA_UNIT_TESTS == 1