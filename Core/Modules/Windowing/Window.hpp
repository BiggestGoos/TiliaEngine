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

		using callback_ptr = void*;
		using property_ptr = void*;

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
			if (i != callback_count)
				callbacks.erase(callbacks.begin() + i);
		}

		void Make_Context_Current() const;

		void Swap_Buffers() const;

		template<typename T, bool Access_Type = T::Access_Type,
			std::enable_if_t<std::is_same<decltype(T::Type),
			const enums::Window_Properties>::value && ((Access_Type == properties::Setter)? 
				!std::is_same<typename T::First_Setter_Param, nullptr_t>::value : 
				!std::is_same<typename T::First_Getter_Param, nullptr_t>::value)
			>* = nullptr>
			void Property (T&& property)
		{
			if (Access_Type == properties::Setter)
			{
				Set_Property(T::Type, static_cast<void*>(&property.set_parameters));
			}
			else
			{
				Get_Property(T::Type, static_cast<void*>(&property.get_parameters));
			}
		}

		//template<template<typename> typename T, bool Access_Type = properties::Setter,
		//	std::enable_if_t<std::is_same<decltype(T<Access_Type>::Type),
		//	const enums::Window_Properties>::value && 
		//	!std::is_same<typename T<Access_Type>::First_Setter_Param, nullptr_t>::value>* = nullptr>
		//	void Set_Property(T<Access_Type> property)
		//{
		//	std::cout << std::get<0>(property.set_parameters) << " : " << std::get<1>(property.set_parameters) << '\n';
		//}

		//template<template<typename> typename T, typename Access_Type = properties::Getter,
		//	std::enable_if_t<std::is_same<decltype(T<Access_Type>::Type),
		//	const enums::Window_Properties>::value &&
		//	!std::is_same<typename T<Access_Type>::First_Getter_Param, nullptr_t>::value>* = nullptr >
		//	void Get_Property(T<Access_Type> property)
		//{
		//	std::get<0>(property.get_parameters) = 67;
		//	std::get<1>(property.get_parameters) = 734;
		//	std::get<2>(property.get_parameters) = 37;
		//	std::get<3>(property.get_parameters) = 956;
		//}

		template<enums::Window_Properties property, typename... Parameters>
		void Set(const Parameters&... args) 
		{ static_assert(false, "Basic Set function is not callable"); }

		template<enums::Window_Properties property, typename... Parameters>
		void Get(Parameters... args)
		{ static_assert(false, "Basic Get function is not callable"); }

		template<>
		void Set<enums::Window_Properties::Should_Close>(const bool& should_close)
		{
			properties::Set_Should_Close_Parameters params{ should_close };
			Set_Property(enums::Window_Properties::Should_Close, &params);
		}

		template<>
		void Get<enums::Window_Properties::Should_Close>(bool& should_close)
		{
			properties::Get_Should_Close_Parameters params{ should_close };
			Get_Property(enums::Window_Properties::Should_Close, &params);
		}

		//template<>
		//void Set<enums::Window_Properties::Size>(std::int32_t width, std::int32_t height);

		//template<>
		//void Set<enums::Window_Properties::Size_Limits>(std::int32_t min_width, 
		//	std::int32_t min_height, std::int32_t max_width, std::int32_t max_height);

		//template<>
		//void Set<enums::Window_Properties::Aspect_Ratio>(std::int32_t numer, std::int32_t denom);

		//template<>
		//void Set<enums::Window_Properties::Position>(std::int32_t x_pos, std::int32_t y_pos);

		//template<>
		//void Set<enums::Window_Properties::Title>(std::string title);

		//template<>
		//void Set<enums::Window_Properties::Icon>();

		//template<>
		//void Set<enums::Window_Properties::Monitor>();

		//template<>
		//void Set<enums::Window_Properties::Iconify>(bool iconify);

		//template<>
		//void Set<enums::Window_Properties::Maximize>(bool maximize);

		//template<>
		//void Set<enums::Window_Properties::Visible>(bool visible);

		//template<>
		//void Set<enums::Window_Properties::Focus>();

		//template<enums::Window_Properties property, typename... Parameters>
		//void Get(Parameters... args) { }

		//template<>
		//void Get<enums::Window_Properties::Should_Close>();

		utils::GLFWwindow* Get_Window() { return m_window; }

	private:

		void Set_Callback(const enums::Window_Callbacks& type, callback_ptr callback);

		void Set_Property(const enums::Window_Properties& type, void* property);
		void Get_Property(const enums::Window_Properties& type, void* property);

		utils::GLFWwindow* m_window{};

		std::tuple<
			std::vector<callbacks::Position::Signature>, 
			std::vector<callbacks::Size::Signature>,
			std::vector<callbacks::Close::Signature>,
			std::vector<callbacks::Refresh::Signature>,
			std::vector<callbacks::Focus::Signature>,
			std::vector<callbacks::Inconify::Signature>,
			std::vector<callbacks::Maximize::Signature>,
			std::vector<callbacks::Framebuffer_Size::Signature>,
			std::vector<callbacks::Content_Scale::Signature>
		> m_callbacks;

		static std::unordered_map<utils::GLFWwindow*, Window&> s_windows;

		void Set_Should_Close(const bool& should_close);

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
