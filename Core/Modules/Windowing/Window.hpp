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

// Vendor
#include "vendor/glm/include/glm/glm.hpp"

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

			friend struct properties::Window_Property<enums::Property::Title>;
			friend struct properties::Window_Property<enums::Property::Focus>;
			friend struct properties::Window_Property<enums::Property::Swap_Interval>;
			friend struct properties::Window_Property<enums::Property::Monitor>;
			friend struct properties::Window_Property<enums::Property::Fullscreen>;

			using callback_ptr = void*;
			using property_ptr = void*;

			template<typename Func>
			bool is_same_function(Func& f1, Func& f2) {
				return f1.target_type() == f2.target_type() &&
					*reinterpret_cast<void**>(&f1) == *reinterpret_cast<void**>(&f2);
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
				const enums::Callback>::value>* = nullptr>
				void Add_Callback(T callback)
			{
				std::get<enums::operator*(T::Type)>(m_callbacks).push_back(std::move(callback));
			}

			template<typename T,
				std::enable_if_t<std::is_same<decltype(T::Type),
				const enums::Callback>::value>* = nullptr>
				void Remove_Callback(T callback)
			{
				auto& callbacks{ std::get<enums::operator*(T::Type)>(m_callbacks) };
				const auto callback_count{ callbacks.size() };
				std::size_t i{ 0 };
				for (; i < callback_count; ++i)
				{
					if (is_same_function(callback.function, callbacks[i].function))
						break;
				}
				if (i < callback_count)
					callbacks.erase(callbacks.begin() + i);
			}

			void Make_Context_Current() const;

			void Swap_Buffers() const;

			template<enums::Property Type, typename Property = properties::Window_Property<Type>>
			void Set(typename Property::Set_Parameters parameters)
			{
				Property::Set(*this, std::move(parameters));
			}

			template<enums::Property Type, typename Property = properties::Window_Property<Type>>
			void Set()
			{
				Property::Set(*this);
			}

			template<enums::Property Type, typename Property = properties::Window_Property<Type>>
			auto Get()
			{
				return Property::Get(*this);
			}

			template<>
			auto Get<enums::Property::Underlying_Window>()
			{
				return m_window;
			}

		protected:

			std::string m_title{ "GLFW" };

			std::int32_t m_swap_interval{ 1 };

			std::pair<std::pair<std::uint32_t, std::uint32_t>, 
				std::pair<std::uint32_t, std::uint32_t>> m_stored_area{};

		private:

			void Set_Callback(enums::Callback type, callback_ptr callback);

			GLFWwindow* m_window{};

			std::tuple<
				std::vector<callbacks::Position>,
				std::vector<callbacks::Size>,
				std::vector<callbacks::Close>,
				std::vector<callbacks::Refresh>,
				std::vector<callbacks::Focus>,
				std::vector<callbacks::Inconify>,
				std::vector<callbacks::Maximize>,
				std::vector<callbacks::Framebuffer_Size>,
				std::vector<callbacks::Content_Scale>
			> m_callbacks;

			static GLFWwindow* s_hidden_window;

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