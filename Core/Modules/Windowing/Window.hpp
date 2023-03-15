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
#include TILIA_WINDOW_PROPERTIES_INCLUDE

namespace tilia
{

	class Window
	{
	private:

		friend struct windowing::properties::Swap_Interval;

		using callback_ptr = void*;
		using property_ptr = void*;

		template<typename T, typename... U>
		std::size_t Get_Address(std::function<T(U...)> func) {
			using func_type = T(*)(U...);
			func_type* func_ptr = func.target<func_type>();
			return *static_cast<std::size_t*>(static_cast<void*>(*func_ptr));
		}

	public:

		static void Set_Window_Hint(enums::Window_Hints hint_type, 
			std::int32_t hint_value);

		static void Init();

		static void Terminate();

		~Window();

		void Init(std::int32_t width, std::int32_t height, std::string title,
			void* monitor, void* share);

		void Destroy();

		template<typename T,
			std::enable_if_t<std::is_same<decltype(T::Type), 
			const enums::Window_Callbacks>::value>* = nullptr>
		void Add_Callback(T callback)
		{	
			std::get<enums::operator*(T::Type)>(m_callbacks).push_back(callback.function);
		}

		template<typename T,
			std::enable_if_t<std::is_same<decltype(T::Type), 
			const enums::Window_Callbacks>::value>* = nullptr>
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
			if (i < callback_count)
				callbacks.erase(callbacks.begin() + i);
		}

		void Make_Context_Current() const;

		void Swap_Buffers() const;

		template<typename T,
			std::enable_if_t<T::Settable == true>* = nullptr>
			void Set(T property)
		{
			property.Set_Property(*this);
		}

		template<typename T,
			std::enable_if_t<T::Gettable == true && 
			(std::tuple_size<typename T::Get_Parameters_Tuple>::value > 1)>* = nullptr>
			auto Get(T property)
		{
			property.Get_Property(*this);
			return property.Get_Properties();
		}

		template<typename T,
			std::enable_if_t<T::Gettable == true &&
			(std::tuple_size<typename T::Get_Parameters_Tuple>::value == 1)>* = nullptr>
			auto Get(T property)
		{
			property.Get_Property(*this);
			return property.Get_First_Property();
		}

		utils::GLFWwindow* Get_Window() { return m_window; }

	protected:

		std::int32_t m_swap_interval{ 1 };

	private:

		void Set_Callback(enums::Window_Callbacks type, callback_ptr callback);

		utils::GLFWwindow* m_window{};

		std::tuple<
			std::vector<windowing::callbacks::Position::Signature>, 
			std::vector<windowing::callbacks::Size::Signature>,
			std::vector<windowing::callbacks::Close::Signature>,
			std::vector<windowing::callbacks::Refresh::Signature>,
			std::vector<windowing::callbacks::Focus::Signature>,
			std::vector<windowing::callbacks::Inconify::Signature>,
			std::vector<windowing::callbacks::Maximize::Signature>,
			std::vector<windowing::callbacks::Framebuffer_Size::Signature>,
			std::vector<windowing::callbacks::Content_Scale::Signature>
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
