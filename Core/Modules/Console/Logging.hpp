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
#include <iostream>

namespace tilia {

	namespace log {

		class Logger
		{
		public:

			static Logger& Instance()
			{
				static Logger logger{};
				return logger;
			}

			static void OpenGL_Error_Callback(std::uint32_t source, std::uint32_t type, 
				std::uint32_t id, std::int32_t length, 
				const char* message, const void* user_param);

			static void GLFW_Error_Callback(std::int32_t error_code, const char* description);

		private:

			Logger();

			std::weak_ptr<std::ostream> m_output{  };

		}; // Logger

	} // log

} // tilia

#endif // TILIA_LOGGING_HPP