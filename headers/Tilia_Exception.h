/*****************************************************************//**
 * @file   Tilia_Exception.h
 * @brief  An exception class deriving from @include <stdexcept> 
 * @exception
 * 
 * @define TILIA_TILIA_EXCEPTION_H
 * 
 * Standard:
 * @include <stdexcept>
 * 
 * @author Gustav Fagerlind
 * @date   10/07/2022
 *********************************************************************/

#ifndef TILIA_TILIA_EXCEPTION_H
#define TILIA_TILIA_EXCEPTION_H

// Standard
#include <stdexcept>

namespace tilia {

	namespace utils {

		class Tilia_Exception 
			: public std::exception 
		{
		public:

			explicit Tilia_Exception(const std::string& message) : exception(message.c_str()) {}

			explicit Tilia_Exception(const char* message) : exception(message) {}

		};

	}

}

#endif