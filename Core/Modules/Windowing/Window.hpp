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

	namespace windowing
	{

		class Window
		{
		private:

			friend struct properties::Underlying_Window;
			friend struct properties::Title;
			friend struct properties::Swap_Interval;

			using callback_ptr = void*;
			using property_ptr = void*;

			template<typename T, typename... U>
			std::size_t Get_Address(std::function<T(U...)> func) {
				using func_type = T(*)(U...);
				func_type* func_ptr = func.target<func_type>();
				return *static_cast<std::size_t*>(static_cast<void*>(*func_ptr));
			}

		public:

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

			template<enums::Window_Properties Property, typename... Types, typename T = properties::Set_Window_Property<Property>,
				std::enable_if_t<std::is_same<std::tuple<Types...>, typename T::Tuple>::type>* = nullptr>
				void Set(Types... arguments)
			{
				T::Set(*this, arguments...);
			}

			template<typename T,
				std::enable_if_t<T::Settable == true>* = nullptr>
				void Set(T property)
			{
				property.Set(*this);
			}

			template<typename T,
				std::enable_if_t<T::Gettable == true>* = nullptr>
				auto Get(T property)
			{
				property.Get(*this);
				constexpr bool multiple_values{ (std::tuple_size<typename T::Get_Parameters_Tuple>::value > 1) };
				return property.Get_Value<multiple_values>();
			}

		protected:

			std::string m_title{ "GLFW" };

			std::int32_t m_swap_interval{ 1 };

		private:

			void Set_Callback(enums::Window_Callbacks type, callback_ptr callback);

			GLFWwindow* m_window{};

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

			static std::unordered_map<GLFWwindow*, Window&> s_windows;

			static void Position_Func(GLFWwindow* window, std::int32_t x_pos, std::int32_t y_pos);

			static void Size_Func(GLFWwindow* window, std::int32_t width, std::int32_t height);

			static void Close_Func(GLFWwindow* window);

			static void Refresh_Func(GLFWwindow* window);

			static void Focus_Func(GLFWwindow* window, std::int32_t focused);

			static void Inconify_Func(GLFWwindow* window, std::int32_t iconified);

			static void Maximize_Func(GLFWwindow* window, std::int32_t maximized);

			static void Framebuffer_Size_Func(GLFWwindow* window, std::int32_t width, std::int32_t height);

			static void Content_Scale_Func(GLFWwindow* window, float x_scale, float y_scale);

		}; // Window

	} // windowing

} // tilia

#endif // TILIA_WINDOW_HPP
