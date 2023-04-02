/**************************************************************************************************
 * @file   Monitor.hpp
 * 
 * @brief  Abstracts the GLFWmonitor object into a class. Can do all the things that the base
 *		   monitor can do.
 * 
 * @author Gustav Fagerlind
 * @date   31/03/2023
 *************************************************************************************************/

#ifndef TILIA_MONITOR_HPP
#define TILIA_MONITOR_HPP

// Standard
#include <unordered_set>
#include <vector>
#include <functional>
#include <set>

// Tilia
#include "Core/Values/Directories.hpp"
#include TILIA_CONSTANTS_INCLUDE

namespace tilia
{

	namespace windowing
	{

		class Window; // Window

	} // windowing

	namespace monitoring
	{

		typedef struct GLFWmonitor GLFWmonitor; // GLFWmonitor

		class Monitor; // Monitor

		// Type for containing a gamma ramp for red, green, and blue values
		using Gamma_Ramp = std::vector<std::tuple<std::uint16_t, std::uint16_t, std::uint16_t>>;

		namespace callbacks
		{
			
			/**
			 * @brief Called everytime that a monitor is connected or disconnected.
			 * 
			 * @param monitor - The monitor that is connected or disconnected.
			 * @param event - Whether the monitor was connected or disconnected.
			 */
			struct Monitor_Func
			{
				using Parameters = void(GLFWmonitor*, std::int32_t);
				using Signature = std::function<Parameters>;
				Signature function{};
				Monitor_Func(Signature func) : function{ func } { }
			}; // Monitor_Func

		} // callbacks

		namespace properties
		{

			template<enums::Monitor_Properties Type>
			struct Monitor_Property {};

			/**
			 * @brief The resolution of the screen in screen coordinates.
			 */
			template<>
			struct Monitor_Property<enums::Monitor_Properties::Resolution>
			{
				using Get_Parameters = std::pair<std::uint32_t, std::uint32_t>;
				static Get_Parameters Get(Monitor& monitor);
			};
			/**
			 * @brief Bit depths of the RGB values.
			 */
			template<>
			struct Monitor_Property<enums::Monitor_Properties::Bit_Depths>
			{
				using Get_Parameters = std::tuple<std::uint32_t, std::uint32_t, std::uint32_t>;
				static Get_Parameters Get(Monitor& monitor);
			};
			/**
			 * @brief Refresh rate of monitor in Hz.
			 */
			template<>
			struct Monitor_Property<enums::Monitor_Properties::Refresh_Rate>
			{
				using Get_Parameters = std::uint32_t;
				static Get_Parameters Get(Monitor& monitor);
			};
			/**
			 * @brief All supported resolutions of the monitor. 
			 */
			template<>
			struct Monitor_Property<enums::Monitor_Properties::Supported_Resolutions>
			{
				using Get_Parameters = std::set<std::pair<std::uint32_t, std::uint32_t>>;
				static Get_Parameters Get(Monitor& monitor);
			};
			/**
			 * @brief All supported bit depths of the monitor at the resolution.
			 */
			template<>
			struct Monitor_Property<enums::Monitor_Properties::Supported_Bit_Depths>
			{
				using Get_Parameters = 
					std::vector<std::tuple<std::uint32_t, std::uint32_t, std::uint32_t>>;
				using Get_Arguments = 
					Monitor_Property<enums::Monitor_Properties::Resolution>::Get_Parameters;
				static Get_Parameters Get(Monitor& monitor, Get_Arguments&& get_arguments);
			};
			/**
			 * @brief All supported refresh rates of the monitor at the resolution.
			 */
			template<>
			struct Monitor_Property<enums::Monitor_Properties::Supported_Refresh_Rates>
			{
				using Get_Parameters = std::vector<std::uint32_t>;
				using Get_Arguments = 
					Monitor_Property<enums::Monitor_Properties::Resolution>::Get_Parameters;
				static Get_Parameters Get(Monitor& monitor, Get_Arguments&& get_arguments);
			};
			/**
			 * @brief An approximation of the physical size of the monitor.
			 */
			template<>
			struct Monitor_Property<enums::Monitor_Properties::Physical_Size>
			{
				using Get_Parameters = std::pair<std::int32_t, std::int32_t>;
				static Get_Parameters Get(Monitor& monitor);
			};
			/**
			 * @brief Ratio of monitor DPI and platform default DPI.
			 */
			template<>
			struct Monitor_Property<enums::Monitor_Properties::Content_Scale>
			{
				using Get_Parameters = std::pair<float, float>;
				static Get_Parameters Get(Monitor& monitor);
			};
			/**
			 * @brief The position of the monitor on the virtual desktop in screen coordinates.
			 */
			template<>
			struct Monitor_Property<enums::Monitor_Properties::Virtual_Position>
			{
				using Get_Parameters = std::pair<std::int32_t, std::int32_t>;
				static Get_Parameters Get(Monitor& monitor);
			};
			/**
			 * @brief The are of the monitor not used by global task bars or menu bars in screen
			 * coordinates.
			 */
			template<>
			struct Monitor_Property<enums::Monitor_Properties::Workarea>
			{
				using Get_Parameters = 
					std::tuple<std::int32_t, std::int32_t, std::int32_t, std::int32_t>;
				static Get_Parameters Get(Monitor& monitor);
			};
			/**
			 * @brief The name of the monitor.
			 */
			template<>
			struct Monitor_Property<enums::Monitor_Properties::Name>
			{
				using Get_Parameters = std::string;
				static Get_Parameters Get(Monitor& monitor);
			};
			/**
			 * @brief The gamma ramp of the monitor.
			 */
			template<>
			struct Monitor_Property<enums::Monitor_Properties::Gamma_Ramp>
			{
				using Set_Parameters = Gamma_Ramp;
				using Get_Parameters = Gamma_Ramp;
				static void Set(Monitor& monitor, Set_Parameters&& parameters);
				static Get_Parameters Get(Monitor& monitor);
			};
			/**
			 * @brief A simpler version of the gamma ramp.
			 */
			template<>
			struct Monitor_Property<enums::Monitor_Properties::Gamma>
			{
				using Set_Parameters = float;
				static void Set(Monitor& monitor, Set_Parameters&& parameters);
			};

		} // properties

		/**
		 * @brief Abstraction for GLFWmonitor. Objects of this class can be gotten via the static
		 * 'Get_Monitors' function. 
		 */
		class Monitor
		{
		public:

			/**
			 * @brief Creates an object of Monitor using a GLFWmonitor pointer. Since all values
			 * are queried from the pointer internally this object is the same as all other objects
			 * holding this pointer. 
			 */
			Monitor(GLFWmonitor* monitor) : m_monitor{ monitor } { }

			/**
			 * @brief Gets all the currently connected monitors as of when this function is called.
			 */
			static std::vector<Monitor> Get_Monitors();

			/**
			 * @brief Adds the given callback which will then be called everytime a monitor is
			 * disconnected or connected.
			 */
			static void Add_Callback(callbacks::Monitor_Func callback)
			{
				s_callbacks.push_back(callback);
			}
			/**
			 * @brief Removes the given callback which will stop it from being called.
			 */
			static void Remove_Callback(callbacks::Monitor_Func callback);

			/**
			 * @brief Whether or not this monitor is connected to the computer.
			 */
			bool Is_Connected() const;

			/**
			 * @brief Sets the property of the given type with the given parameters.
			 */
			template<enums::Monitor_Properties Type, 
				typename Property = properties::Monitor_Property<Type>>
			void Set(typename Property::Set_Parameters parameters)
			{
				Property::Set(*this, std::move(parameters));
			}

			/**
			 * @brief Gets the property of the given type.
			 */
			template<enums::Monitor_Properties Type, 
				typename Property = properties::Monitor_Property<Type>>
			auto Get()
			{
				return Property::Get(*this);
			}

			/**
			 * @brief Gets the property of the given type based on the given arguments.
			 */
			template<enums::Monitor_Properties Type, 
				typename Property = properties::Monitor_Property<Type>>
			auto Get(typename Property::Get_Arguments arguments)
			{
				return Property::Get(*this, std::move(arguments));
			}

			/**
			 * @brief Gets the GLFWmonitor pointer.
			 */
			template<>
			auto Get<enums::Monitor_Properties::Underlying_Monitor>()
			{
				return m_monitor;
			}

#if TILIA_UNIT_TESTS == 1

			/**
			 * @brief Unit test for Monitor.
			 */
			static void Test();

#endif // TILIA_UNIT_TESTS == 1

		private:

			friend class tilia::windowing::Window;

			// Pointer to the GLFWmonitor which is used to access and change information about the
			// monitor
			GLFWmonitor* m_monitor{};

			/**
			 * @brief Sets up all the monitoring stuff.
			 */
			static void Init();

			// A list with all the monitors connected to the computer
			static std::set<Monitor> s_monitors;

			// A list of callbacks which are called in the 'Monitor_Func' callback
			static std::vector<callbacks::Monitor_Func> s_callbacks;

			// Callback function which is called every time a monitor is connected or disconnected
			// from the computer
			static void Monitor_Func(GLFWmonitor* monitor, std::int32_t event);

		public:

			friend bool operator<(const Monitor& lhs, const Monitor& rhs)
			{
				return (lhs.m_monitor < rhs.m_monitor);
			}

			friend bool operator==(const Monitor& lhs, const Monitor& rhs)
			{
				return (lhs.m_monitor == rhs.m_monitor);
			}
			friend bool operator!=(const Monitor& lhs, const Monitor& rhs)
			{
				return !(lhs == rhs);
			}

		}; // Monitor

	} // monitoring

} // tilia

#endif // TILIA_MONITOR_HPP
