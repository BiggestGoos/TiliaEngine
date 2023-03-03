/**************************************************************************************************
 * @file   Logging.hpp
 * 
 * @brief  Declarations of functions used for logging information to the console.
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
#include <string>
#include <initializer_list>

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
			 * to this instance is returned to anywhere in the program. Should only be called after 
			 * main has started.
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
			 * 
			 * @param filters - The filters to store. If none is given then the filters are set to 
			 * empty.
			 */
			void Set_Filters(std::vector<std::string> filters = {});

			/**
			 * @brief Gets the stored filters.
			 * 
			 * @return The stored filters.
			 */
			std::vector<std::string> Get_Filters() const;

			/**
			 * @brief Prints the given data to the given outputs.
			 * 
			 * @param data - The data to print.
			 * @param outputs - The outputs to print to. If none are given then it prints to all 
			 * stored ones.
			 */
			void Output(const std::string& data, std::vector<std::ostream*> outputs = {});

			/**
			 * @brief Prints the given data to different outputs depending upon filters.
			 *
			 * @param data - The data to print.
			 */
			template<typename... T>
			void Output(T... data)
			{
				std::stringbuf str_buf{};
				std::ostream buffer{ &str_buf };
				((buffer << std::forward<T>(data)), ...);
				Output(str_buf.str());
			}
			
			/**
			 * @brief Prints the given data to different outputs depending upon filters.
			 *
			 * @param data - The data to print.
			 */
			void Output(const utils::Exception_Data& data);

			/**
			* @brief Prints the given data to different outputs depending upon filters.
			*
			* @param data - The data to print.
			*/
			void Output(const utils::Tilia_Exception& data);

			/**
			 * @brief Adds an output and the filters to be used with it.
			 * 
			 * @param output - The output to print to.
			 * @param filters - The filters to be used for this output. If none is given then the 
			 * filters are set to empty.
			 */
			void Add_Output(std::ostream* output, std::vector<std::string> filters = {});

			/**
			 * @brief Removes the given filter from the logger (if it exists).
			 * 
			 * @param output - The output to remove.
			 */
			void Remove_Output(std::ostream* const output);

			/**
			 * @brief Sets the filters of the given output.
			 * 
			 * @param output - The output to set the filters of.
			 * @param filters - The filters to be used for this output. If none is given then the 
			 * filters are set to empty.
			 */
			void Set_Output_Filters(std::ostream* output, 
				std::vector<std::string> filters = {});

			/**
			 * @brief Sets the filters to be used by the openGL callback
			 * 
			 * @param filters - The filters to be used for openGL. If none is given then the 
			 * filters are set to empty.
			 */
			void Set_OpenGL_Filters(std::vector<std::string> filters = {})
			{
				m_openGL_filters = filters;
			}

			/**
			 * @brief An error callback which can be used with openGL. Should not be called.
			 */
			static void OpenGL_Error_Callback(std::uint32_t source, std::uint32_t type, 
				std::uint32_t severity, std::uint32_t id, std::int32_t length,
				const char* message, const void* user_param);

			/**
			 * @brief Sets the filters to be used by the GLFW callback
			 *
			 * @param filters - The filters to be used for GLFW. If none is given then the
			 * filters are set to empty.
			 */
			void Set_GLFW_Filters(std::vector<std::string> filters = {})
			{
				m_GLFW_filters = filters;
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

			Logger();

			// The different outputs and their respective filters
			std::vector<std::pair<std::ostream*, std::vector<std::string>>> m_outputs{};
			// Filters to be used by the openGL error callback
			std::vector<std::string> m_openGL_filters{};
			// Filters to be used by the GLFW error callback
			std::vector<std::string> m_GLFW_filters{};
			// The filters of the logger to be compared with the outputs' filters
			std::vector<std::string> m_filters{};
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