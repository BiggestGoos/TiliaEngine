#include "Core/Temp/Input.hpp"

void tilia::Input_Manager::Init(GLFWwindow* window)
{

	m_window = window;

	glfwSetCursorPosCallback(m_window, utils::Mouse_Pos_Callback);
	glfwSetScrollCallback(m_window, utils::Mouse_Scroll_Callback);
	glfwSetKeyCallback(m_window, utils::Key_Press_Callback);
	glfwSetMouseButtonCallback(m_window, utils::Mouse_Click_Callback);

}

void tilia::Input_Manager::Update()
{

	m_last_frame_mouse_pos = m_mouse_pos;

	m_mouse_pos = { utils::x_pos, utils::y_pos };

	m_scroll_offset = { utils::x_scroll_offset, utils::y_scroll_offset };

	utils::x_scroll_offset = 0;
	utils::y_scroll_offset = 0;

	m_current_key_scancode = utils::key_scancode;
	m_current_key_action = utils::key_action;

	utils::key_scancode = -1;
	utils::key_action = -1;

	m_current_mouse_button_type = utils::mouse_button_type;
	m_current_mouse_button_action = utils::mouse_button_action;

	utils::mouse_button_type = -1;
	utils::mouse_button_action = -1;

	if (Get_Key_Pressed(m_exit_key))
		glfwSetWindowShouldClose(m_window, true);

}
