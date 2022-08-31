
#ifndef S2D_INPUT_H
#define S2D_INPUT_H

#include "vendor/glfw/include/GLFW/glfw3.h"

#include <iostream>

#include "vendor/glm/include/glm/glm.hpp"

namespace tilia {

#ifndef S2D_KEYS
#define S2D_KEYS

	/* The unknown key */
#define KEY_UNKNOWN            -1

/* Printable keys */
#define KEY_SPACE              32  /*   */
#define KEY_APOSTROPHE         39  /* ' */
#define KEY_COMMA              44  /* , */
#define KEY_MINUS              45  /* - */
#define KEY_PERIOD             46  /* . */
#define KEY_SLASH              47  /* / */
#define KEY_0                  48
#define KEY_1                  49
#define KEY_2                  50
#define KEY_3                  51
#define KEY_4                  52
#define KEY_5                  53
#define KEY_6                  54
#define KEY_7                  55
#define KEY_8                  56
#define KEY_9                  57
#define KEY_SEMICOLON          59  /* ; */
#define KEY_EQUAL              61  /* = */
#define KEY_A                  65
#define KEY_B                  66
#define KEY_C                  67
#define KEY_D                  68
#define KEY_E                  69
#define KEY_F                  70
#define KEY_G                  71
#define KEY_H                  72
#define KEY_I                  73
#define KEY_J                  74
#define KEY_K                  75
#define KEY_L                  76
#define KEY_M                  77
#define KEY_N                  78
#define KEY_O                  79
#define KEY_P                  80
#define KEY_Q                  81
#define KEY_R                  82
#define KEY_S                  83
#define KEY_T                  84
#define KEY_U                  85
#define KEY_V                  86
#define KEY_W                  87
#define KEY_X                  88
#define KEY_Y                  89
#define KEY_Z                  90
#define KEY_LEFT_BRACKET       91  /* [ */
#define KEY_BACKSLASH          92  /* \ */
#define KEY_RIGHT_BRACKET      93  /* ] */
#define KEY_GRAVE_ACCENT       96  /* ` */
#define KEY_WORLD_1            161 /* non-US #1 */
#define KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define KEY_ESCAPE             256
#define KEY_ENTER              257
#define KEY_TAB                258
#define KEY_BACKSPACE          259
#define KEY_INSERT             260
#define KEY_DELETE             261
#define KEY_RIGHT              262
#define KEY_LEFT               263
#define KEY_DOWN               264
#define KEY_UP                 265
#define KEY_PAGE_UP            266
#define KEY_PAGE_DOWN          267
#define KEY_HOME               268
#define KEY_END                269
#define KEY_CAPS_LOCK          280
#define KEY_SCROLL_LOCK        281
#define KEY_NUM_LOCK           282
#define KEY_PRINT_SCREEN       283
#define KEY_PAUSE              284
#define KEY_F1                 290
#define KEY_F2                 291
#define KEY_F3                 292
#define KEY_F4                 293
#define KEY_F5                 294
#define KEY_F6                 295
#define KEY_F7                 296
#define KEY_F8                 297
#define KEY_F9                 298
#define KEY_F10                299
#define KEY_F11                300
#define KEY_F12                301
#define KEY_F13                302
#define KEY_F14                303
#define KEY_F15                304
#define KEY_F16                305
#define KEY_F17                306
#define KEY_F18                307
#define KEY_F19                308
#define KEY_F20                309
#define KEY_F21                310
#define KEY_F22                311
#define KEY_F23                312
#define KEY_F24                313
#define KEY_F25                314
#define KEY_KP_0               320
#define KEY_KP_1               321
#define KEY_KP_2               322
#define KEY_KP_3               323
#define KEY_KP_4               324
#define KEY_KP_5               325
#define KEY_KP_6               326
#define KEY_KP_7               327
#define KEY_KP_8               328
#define KEY_KP_9               329
#define KEY_KP_DECIMAL         330
#define KEY_KP_DIVIDE          331
#define KEY_KP_MULTIPLY        332
#define KEY_KP_SUBTRACT        333
#define KEY_KP_ADD             334
#define KEY_KP_ENTER           335
#define KEY_KP_EQUAL           336
#define KEY_LEFT_SHIFT         340
#define KEY_LEFT_CONTROL       341
#define KEY_LEFT_ALT           342
#define KEY_LEFT_SUPER         343
#define KEY_RIGHT_SHIFT        344
#define KEY_RIGHT_CONTROL      345
#define KEY_RIGHT_ALT          346
#define KEY_RIGHT_SUPER        347
#define KEY_MENU               348

#define KEY_LAST               GLFW_KEY_MENU

#endif

#ifndef S2D_MOUSE_BUTTONS
#define S2D_MOUSE_BUTTONS

/* Mouse buttons */
#define MOUSE_BUTTON_1         0
#define MOUSE_BUTTON_2         1
#define MOUSE_BUTTON_3         2
#define MOUSE_BUTTON_4         3
#define MOUSE_BUTTON_5         4
#define MOUSE_BUTTON_6         5
#define MOUSE_BUTTON_7         6
#define MOUSE_BUTTON_8         7
#define MOUSE_BUTTON_LAST      MOUSE_BUTTON_8
#define MOUSE_BUTTON_LEFT      MOUSE_BUTTON_1
#define MOUSE_BUTTON_RIGHT     MOUSE_BUTTON_2
#define MOUSE_BUTTON_MIDDLE    MOUSE_BUTTON_3

#endif
	
	namespace utils {

		static double x_pos, y_pos;
		static void Mouse_Pos_Callback(GLFWwindow* window, double xPos, double yPos) {
			x_pos = xPos;
			y_pos = yPos;
		}

		static double x_scroll_offset, y_scroll_offset;
		static void Mouse_Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset) {
			x_scroll_offset = xoffset;
			y_scroll_offset = yoffset;
		}

		static int key_scancode, key_action;
		static void Key_Press_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			key_scancode = scancode;
			key_action = action;
		}

		static int mouse_button_type, mouse_button_action;
		static void Mouse_Click_Callback(GLFWwindow* window, int button, int action, int mods) {
			mouse_button_type = button;
			mouse_button_action = action;
		}

	}

	class Input_Manager {
	private:

		GLFWwindow* m_window{ nullptr };

		glm::vec2 m_last_frame_mouse_pos{};
		glm::vec2 m_mouse_pos{};

		glm::vec2 m_scroll_offset{};

		int m_exit_key{ KEY_ESCAPE };

		int m_current_key_scancode{ -1 },
			m_current_key_action{ -1 };

		int m_current_mouse_button_type{ -1 },
			m_current_mouse_button_action{ -1 };

	public:

		void Init(GLFWwindow* window);

		void Update();

		inline glm::vec2 Get_Mouse_Pos() const { 
			return m_mouse_pos;
		}

		inline glm::vec2 Get_Mouse_Pos_Delta() const { return m_mouse_pos - m_last_frame_mouse_pos; }

		inline int Get_Mouse_Scroll() const { return (int)m_scroll_offset.y; }

		inline bool Get_Mouse_Button_Down(int button) { return glfwGetMouseButton(m_window, button); }

		inline bool Get_Mouse_Button_Pressed(int button) {
			if (button == m_current_mouse_button_type && m_current_mouse_button_action == 1) {
				return true;
			}
			return false;
		}

		inline bool Get_Mouse_Button_Released(int button) {
			if (button == m_current_mouse_button_type && m_current_mouse_button_action == 0) {
				return true;
			}
			return false;
		}

		inline bool Get_Key_Down(int key) { return glfwGetKey(m_window, key); }

		inline bool Get_Key_Pressed(int key) {
			int scancode = glfwGetKeyScancode(key);
			if (scancode == m_current_key_scancode && m_current_key_action == 1) {
				return true;
			}
			return false;
		}

		inline bool Get_Key_Released(int key) {
			int scancode = glfwGetKeyScancode(key);
			if (scancode == m_current_key_scancode && m_current_key_action == 0) {
				return true;
			}
			return false;
		}

		inline void Set_Exit_Key(int key) { m_exit_key = key; }

	};



}

#endif
