/*****************************************************************//**
 * @file   Logging.h
 * @brief  Declarations of functions used for logging information to the console
 * 
 * @define TILIA_LOGGING_H
 * 
 * @enum class Type:
 * INFO = 0
 * DEBUG = 1
 * SUCCESS = 2
 * ERROR = 3
 * 
 * Standard:
 * @include <cstdint>
 * 
 * @author Gustav Fagerlind
 * @date   16/05/2022
 *********************************************************************/

#ifndef TILIA_LOGGING_H
#define TILIA_LOGGING_H

// Standard
#include <cstdint>

namespace tilia {
	namespace log {

		// Log types:
		enum class Type {
			INFO = 0,
			DEBUG = 1,
			SUCCESS = 2,
			ERROR = 3
		};

		/**
		 * @brief A log function which prints what type, tag and text is given.
		 * 
		 * @param log_type - The type to print to the console
		 * @param log_tag  - The tag to print to the console
		 * @param text...  - The text to print to the console
		 */
		void Log(Type log_type, const char* log_tag, const char* text...);

		/**
		 * @brief A log function which can be used as an add-on for the regular Log function. 
		 * It prints a tag and a text with an indentation.
		 * 
		 * @param log_tag - The tag to print to the console
		 * @param text... - The text to print to the console
		 */
		void Log_Indent(const char* log_tag, const char* text...);

	}
}

#endif