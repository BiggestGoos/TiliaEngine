// Standard
#include <cstdio>
#include <cstdarg>

// Tilia
#include "Core/Modules/Console/Logging.hpp"

static void Print_Log_Type(const tilia::log::Type& log_type) {

	switch (log_type) {
	case tilia::log::Type::INFO:
		std::printf(":INFO:");
		return;
	case tilia::log::Type::DEBUG:
		std::printf(":DEBUG:");
		return;
	case tilia::log::Type::SUCCESS:
		std::printf(":SUCCESS:");
		return;
	case tilia::log::Type::ERROR:
		std::printf(":ERROR:");
		return;
	}

}

void tilia::log::Log(const Type& log_type, const char* log_tag, const char* text...)
{

	Print_Log_Type(log_type);

	// Prints opening brackets, and log_tag
	std::printf(":%s: [ ", log_tag);
	
	std::va_list args;

	// Gets the values from the text
	va_start(args, text);

	// Prints text to stdout
	std::vfprintf(stdout, text, args);

	va_end(args);

	// Prints closing brackets
	std::printf(" ]\n");

}

void tilia::log::Log_Indent(const char* log_tag, const char* text ...)
{

	// Prints indentation, log_tag, and opening braces
	std::printf("  >:%s: { ", log_tag);

	std::va_list args;

	// Gets the values from the text
	va_start(args, text);

	// Prints text to stdout
	std::vfprintf(stdout, text, args);

	va_end(args);

	// Prints closing braces
	std::printf(" }\n");

}
