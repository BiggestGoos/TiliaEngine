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

// Tilia
#include "Core/Values/Directories.hpp"
#include TILIA_TILIA_EXCEPTION_INCLUDE

namespace tilia {

	namespace log {

		/**
		 * @brief Singleton logger class. Holds error callback functions for certain APIs which	
		 * can be set. The ostream the logger outputs to can be changed.
		 */
		class Logger
		{
		public:

			static Logger& Instance()
			{
				static Logger logger{};
				return logger;
			}

			void Output(const std::string& data);

			template<typename... T>
			void Output(T... data, std::initializer_list<std::string> filters)
			{
				std::lock_guard lock{ m_mutex };
				if (m_output == nullptr)
					throw utils::Tilia_Exception{ { TILIA_LOCATION, "Logger output is nullptr" } };
				std::stringbuf str_buf{};
				std::ostream buffer{ &str_buf };
				((buffer << std::forward<T>(data)), ...);
				Output(str_buf.str());
			}
			
			void Output(const utils::Exception_Data& data);

			void Output(const utils::Tilia_Exception& data);

			void Add_Output(std::ostream* output, std::initializer_list<std::string> filters) 
			{ 
				std::lock_guard lock{ m_mutex };
				m_output = output; 
			}

			void Remove_Output()

			static void OpenGL_Error_Callback(std::uint32_t source, std::uint32_t type, 
				std::uint32_t severity, std::uint32_t id, std::int32_t length,
				const char* message, const void* user_param);

			static void GLFW_Error_Callback(std::int32_t error_code, const char* description);

		private:

			Logger();

			std::vector<std::pair<std::ostream*, std::vector<std::string>>> m_outputs{};

			mutable std::mutex m_mutex{};

		public:

			// Logger shan't be copyable

			Logger(const Logger& other) = delete;
			Logger(Logger&& other) = delete;

			Logger& operator=(const Logger& other) = delete;
			Logger& operator=(Logger&& other) = delete;

		}; // Logger

	} // log

} // tilia

#endif // TILIA_LOGGING_HPP