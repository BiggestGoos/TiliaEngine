/**************************************************************************************************
 * @file   Window.hpp
 * 
 * @brief  
 * 
 * @author Gustav Fagerlind
 * @date   05/03/2023
 *************************************************************************************************/

#ifndef TILIA_WINDOW_HPP
#define TILIA_WINDOW_HPP

// Standard
#include <string>
#include <utility>
#include <type_traits>
#include <unordered_map>
#include <iostream>

// Tilia
#include "Core/Values/Directories.hpp"
#include TILIA_CONSTANTS_INCLUDE
#include TILIA_WINDOWING_CALLBACKS_INCLUDE

namespace tilia
{

	class Window
	{
	private:

		using callback_ptr = void*;

		template<typename T, typename... U>
		std::size_t Get_Address(std::function<T(U...)> func) {
			using func_type = T(*)(U...);
			func_type* func_ptr = func.target<func_type>();
			return *static_cast<std::size_t*>(static_cast<void*>(*func_ptr));
		}

	public:

		static void Set_Window_Hint(const enums::Window_Hints& hint_type, 
			const std::int32_t& hint_value);

		static void Init();

		static void Terminate();

		~Window();

		void Init(const std::int32_t& width, const std::int32_t& height, const std::string& title,
			void* monitor, void* share);

		void Destroy();

		template<typename T,
			std::enable_if_t<std::is_base_of_v<utils::Window_Func<T::Type, 
			typename T::Parameters>, T>>* = nullptr>
		void Add_Callback(T callback)
		{	
			std::get<enums::operator*(T::Type)>(m_callbacks).push_back(callback.function);
		}

		template<typename T,
			std::enable_if_t<std::is_base_of_v<utils::Window_Func<T::Type,
			typename T::Parameters>, T>>* = nullptr>
			void Remove_Callback(T callback)
		{
			auto& callbacks{ std::get<enums::operator*(T::Type)>(m_callbacks) };
			const auto callback_count{ callbacks.size() };
			std::size_t i{ 0 };
			for (; i < callback_count; ++i)
			{
				if (Get_Address(callback.function) == Get_Address(callbacks[i]))
					break;
			}
			if (i != callback_count)
				callbacks.erase(callbacks.begin() + i);
		}

		void Make_Context_Current() const;

		void Swap_Buffers() const;

		template<enums::Window_Properties property, typename... Parameters>
		void Set(Parameters... args) 
		{ static_assert(false, "Basic Set function is not callable"); }

		template<>
		void Set<enums::Window_Properties::Should_Close>(bool should_close);

		template<>
		void Set<enums::Window_Properties::Size>(std::int32_t width, std::int32_t height);

		template<>
		void Set<enums::Window_Properties::Size_Limits>(std::int32_t min_width, 
			std::int32_t min_height, std::int32_t max_width, std::int32_t max_height);

		template<>
		void Set<enums::Window_Properties::Aspect_Ratio>(std::int32_t numer, std::int32_t denom);

		template<>
		void Set<enums::Window_Properties::Position>(std::int32_t x_pos, std::int32_t y_pos);

		template<>
		void Set<enums::Window_Properties::Title>(std::string title);

		template<>
		void Set<enums::Window_Properties::Icon>();

		template<>
		void Set<enums::Window_Properties::Monitor>();

		template<>
		void Set<enums::Window_Properties::Iconify>(bool iconify);

		template<>
		void Set<enums::Window_Properties::Maximize>(bool maximize);

		template<>
		void Set<enums::Window_Properties::Visible>(bool visible);

		//template<>
		//void Set<enums::Window_Properties::Focus>();

		//template<enums::Window_Properties property, typename... Parameters>
		//void Get(Parameters... args) { }

		//template<>
		//void Get<enums::Window_Properties::Should_Close>();

		utils::GLFWwindow* Get_Window() { return m_window; }

	private:

		void Set_Callback(const enums::Window_Callbacks& type, callback_ptr callback);

		utils::GLFWwindow* m_window{};

		std::tuple<
			std::vector<utils::Position_Func::Signature>, 
			std::vector<utils::Size_Func::Signature>,
			std::vector<utils::Close_Func::Signature>,
			std::vector<utils::Refresh_Func::Signature>,
			std::vector<utils::Focus_Func::Signature>,
			std::vector<utils::Inconify_Func::Signature>,
			std::vector<utils::Maximize_Func::Signature>,
			std::vector<utils::Framebuffer_Size_Func::Signature>,
			std::vector<utils::Content_Scale_Func::Signature>
		> m_callbacks;

		static std::unordered_map<utils::GLFWwindow*, Window&> s_windows;

		static void Position_Func(utils::GLFWwindow* window, std::int32_t x_pos, std::int32_t y_pos);

		static void Size_Func(utils::GLFWwindow* window, std::int32_t width, std::int32_t height);

		static void Close_Func(utils::GLFWwindow* window);

		static void Refresh_Func(utils::GLFWwindow* window);

		static void Focus_Func(utils::GLFWwindow* window, std::int32_t focused);

		static void Inconify_Func(utils::GLFWwindow* window, std::int32_t iconified);

		static void Maximize_Func(utils::GLFWwindow* window, std::int32_t maximized);

		static void Framebuffer_Size_Func(utils::GLFWwindow* window, std::int32_t width, std::int32_t height);

		static void Content_Scale_Func(utils::GLFWwindow* window, float x_scale, float y_scale);

	}; // Window

} // tilia

#endif // TILIA_WINDOW_HPP
