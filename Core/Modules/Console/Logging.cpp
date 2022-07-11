/*****************************************************************//**
 * @file   Logging.cpp
 * @brief  Definitions of functions declared in @include "headers/Logging.h"
 * 
 * Standard:
 * @include <cstdio>
 * @include <cstdarg>
 * 
 * Headers:
 * @include "headers/Logging.h"
 * 
 * @author Gustav Fagerlind
 * @date   16/05/2022
 *********************************************************************/

// Standard
#include <cstdio>
#include <cstdarg>

// Headers
#include "Core/Modules/Console/Logging.h"

/**
 * First prints log_type, then log_tag and then at last prints text using printf, and
 * vfprintf to print to stdout using variadic functions
 */
void tilia::log::Log(Type log_type, const char* log_tag, const char* text...)
{

	// Prints log_type
	switch (log_type) {
	case Type::INFO:
		printf(":INFO:");
		break;
	case Type::DEBUG:
		printf(":DEBUG:");
		break;
	case Type::SUCCESS:
		printf(":SUCCESS:");
		break;
	case Type::ERROR:
		printf(":ERROR:");
		break;
	}

	// Prints opening brackets, and log_tag
	printf(":%s: [ ", log_tag);
	
	// Prints text to stdout
	std::va_list args;

	va_start(args, text);

	vfprintf(stdout, text, args);

	va_end(args);

	// Prints closing brackets
	printf(" ]\n");

}

/**
 * First prints indentation, then log_tag, and at last prints text using printf, and
 * vfprintf to print to stdout using variadic functions. 
 */
void tilia::log::Log_Indent(const char* log_tag, const char* text ...)
{

	// Prints opening braces, log_tag, and indentation
	printf("  >:%s: { ", log_tag);

	// Prints text to stdout
	va_list args;
	va_start(args, text);

	vfprintf(stdout, text, args);

	va_end(args);

	// Prints closing braces
	printf(" }\n");

}
