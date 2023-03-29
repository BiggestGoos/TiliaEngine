/**************************************************************************************************
 * @file   Logging.hpp
 * 
 * @brief  Holds a singleton class which can output messages to given outputs from any thread 
 *		   depending on different filters.
 * 
 * @author Gustav Fagerlind
 * @date   16/05/2022
 *************************************************************************************************/

#ifndef TILIA_LOGGING_HPP
#define TILIA_LOGGING_HPP

// Standard
#include <cstdint>
#include <memory>
#include <sstream>
#include <mutex>
#include <vector>
#include <set>
#include <string>
#include <initializer_list>
#include <algorithm>
#include <unordered_map>

// Tilia
#include "Core/Values/Directories.hpp"
#include TILIA_CONSTANTS_INCLUDE
#include TILIA_TILIA_EXCEPTION_INCLUDE

namespace tilia {

	namespace log {

		/**
		 * @brief Singleton logger class. Holds error callback functions for certain APIs which	
		 * can be set. Can hold many different outputs which can have filters. Should be 
		 * completely thread safe.
		 */
		class Logger
		{
		public:

			/**
			 * @brief First time it is called it will construct an instance of Logger. A reference 
			 * to this instance is returned to anywhere in the program.
			 * 
			 * @return A reference to an instance of Logger.
			 */
			static Logger& Instance()
			{
				static Logger logger{};
				return logger;
			}

			/**
			 * @brief Sets the filtering state that will be used to decide what outputs to print
			 * to.
			 */
			void Set_Filters(const std::set<std::string>& filters = {})
			{
				std::lock_guard lock{ m_mutex };
				m_filters = filters;
			}
			/**
			 * @brief Gets the stored filters.
			 */
			const std::set<std::string>& Get_Filters() const
			{
				std::lock_guard lock{ m_mutex };
				return m_filters;
			}
			/**
			 * @brief Adds a filter to the logger.
			 */
			void Add_Filter(const std::string& filter)
			{
				std::lock_guard lock{ m_mutex };
				m_filters.insert(filter);
			}
			/**
			 * @brief Removes the given filter from the logger.
			 */
			void Remove_Filter(const std::string& filter)
			{
				std::lock_guard lock{ m_mutex };
				m_filters.erase(filter);
			}

			/**
			 * @brief Prints the given data to the stored outputs depending upon filters.
			 * 
			 * @param filters - The filters to be used. If none are given then it uses the stored
			 * ones.
			 */
			void Output(const std::string& data, const std::set<std::string>& filters = {});

			/**
			 * @brief Prints the given data to different outputs depending upon filters.
			 */
			template<typename... Data>
			void Output(Data... data)
			{
				std::stringbuf str_buf{};
				std::ostream buffer{ &str_buf };
				((buffer << std::forward<Data>(data)), ...);
				Output(str_buf.str());
			}
			
			/**
			 * @brief Prints the given data to different outputs depending upon filters.
			 */
			void Output(const utils::Exception_Data& data);

			/**
			* @brief Prints the given data to different outputs depending upon filters.
			*/
			void Output(const utils::Tilia_Exception& data);

			/**
			 * @brief Adds an output and the filters to be used with it.
			 */
			void Add_Output(std::streambuf* output, const std::set<std::string>& filters = {})
			{
				Set_Output_Filters(output, filters);
			}

			/**
			 * @brief Removes the given filter (if it exists) from the logger.
			 */
			void Remove_Output(std::streambuf* const output)
			{
				std::lock_guard lock{ m_mutex };
				m_outputs.erase(output);
			}

			/**
			 * @brief Gets a list of the loggers outputs.
			 */
			auto Get_Outputs() const
			{
				std::lock_guard lock{ m_mutex };
				std::set<std::streambuf*> ret_val{};
				for (const auto& [key, value] : m_outputs)
				{
					ret_val.insert(key);
				}
				return ret_val;
			}

			/**
			 * @brief Sets the filters of the given output.
			 */
			void Set_Output_Filters(std::streambuf* output,
				const std::set<std::string>& filters = {})
			{
				std::lock_guard lock{ m_mutex };
				m_outputs[output] = filters;
			}

			/**
			 * @brief Gets a list of all the filters for the given output.
			 * 
			 * @return The filters of the output. Reference is valid as long as output is stored by
			 * logger.
			 */
			const std::set<std::string>& Get_Output_Filters(std::streambuf* output) const
			{
				std::lock_guard lock{ m_mutex };
				return m_outputs.at(output);
			}

			/**
			 * @brief Adds the given filter to the filters of the given output.
			 */
			void Add_Output_Filter(std::streambuf* output, const std::string& filter)
			{
				std::lock_guard lock{ m_mutex };
				m_outputs[output].insert(filter);
			}

			/**
			 * @brief Removes the given filter from the filters of the given output.
			 */			
			void Remove_Output_Filter(std::streambuf* output, const std::string& filter)
			{
				std::lock_guard lock{ m_mutex };
				m_outputs[output].erase(filter);
			}

			/**
			 * @brief Sets the filters to be used by the openGL callback.
			 */
			void Set_OpenGL_Filters(const std::set<std::string>& filters = {})
			{
				std::lock_guard lock{ m_mutex };
				m_openGL_filters = filters;
			}

			/**
			 * @brief Gets the filters that are used by the openGL callback.
			 */
			const std::set<std::string>& Get_OpenGL_Filters() const
			{
				std::lock_guard lock{ m_mutex };
				return m_openGL_filters;
			}

			/**
			 * @brief Adds the given filter to the filters that are used by the openGL callback.
			 */
			void Add_OpenGL_Filter(const std::string& filter)
			{
				std::lock_guard lock{ m_mutex };
				m_openGL_filters.insert(filter);
			}

			/**
			 * @brief Removes the given filter from the filters that are used by the openGL
			 * callback.
			 */
			void Remove_OpenGL_Filter(const std::string& filter)
			{
				std::lock_guard lock{ m_mutex };
				m_openGL_filters.erase(filter);
			}

			/**
			 * @brief An error callback which can be used with openGL. Should not be called.
			 */
			static void OpenGL_Error_Callback(std::uint32_t source, std::uint32_t type, 
				std::uint32_t severity, std::uint32_t id, std::int32_t length,
				const char* message, const void* user_param);

			/**
			 * @brief Sets the filters to be used by the GLFW callback.
			 */
			void Set_GLFW_Filters(const std::set<std::string>& filters = {})
			{
				std::lock_guard lock{ m_mutex };
				m_GLFW_filters = filters;
			}

			/**
			 * @brief Gets the filters that are used by the GLFW callback.
			 */
			const std::set<std::string>& Get_GLFW_Filters() const
			{
				std::lock_guard lock{ m_mutex };
				return m_GLFW_filters;
			}

			/**
			 * @brief Adds the given filter to the filters that are used by the GLFW callback.
			 */
			void Add_GLFW_Filter(const std::string& filter)
			{
				std::lock_guard lock{ m_mutex };
				m_GLFW_filters.insert(filter);
			}

			/**
			 * @brief Removes the given filter from the filters that are used by the GLFW
			 * callback.
			 */
			void Remove_GLFW_Filter(const std::string& filter)
			{
				std::lock_guard lock{ m_mutex };
				m_GLFW_filters.erase(filter);
			}

			/**
			 * @brief An error callback which can be used with GLFW. Should not be called.
			 */
			static void GLFW_Error_Callback(std::int32_t error_code, const char* description);

#if TILIA_UNIT_TESTS == 1

			/**
			 * @brief Unit test for Logger.
			 */
			static void Test();

#endif

		private:

			Logger() = default;

			// The different outputs and their respective filters
			std::unordered_map<std::streambuf*, std::set<std::string>> m_outputs{};
			// The filters of the logger to be compared with the outputs' filters
			std::set<std::string> m_filters{};
			// Filters to be used by the openGL error callback
			std::set<std::string> m_openGL_filters{};
			// Filters to be used by the GLFW error callback
			std::set<std::string> m_GLFW_filters{};
			// A mutex which is used for the whole logger
			mutable std::mutex m_mutex{};

		public:

			// Logger shan't be copyable or moveable

			Logger(const Logger& other) = delete;
			Logger(Logger&& other) = delete;

			Logger& operator=(const Logger& other) = delete;
			Logger& operator=(Logger&& other) = delete;

		}; // Logger

	} // log

} // tilia

#endif // TILIA_LOGGING_HPP