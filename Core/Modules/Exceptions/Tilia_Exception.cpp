/*****************************************************************//**
 * @file   Tilia_Exception.cpp
 * @brief  Holds member construcors and functions which add messages
 *		   and set the line and file the exception originates from.
 * 
 * Headers:
 * @include "Tilia_Exception.h"
 * 
 * @author Gustav Fagerlind
 * @date   13/07/2022
 *********************************************************************/

// Standard
#include "Tilia_Exception.h"

tilia::utils::Tilia_Exception::Tilia_Exception(const std::string& message, const size_t& line, const std::string& file)
	:
	m_messages{ 1 },
	m_origin_line{ line },
	m_origin_file{ file }
{
	m_messages[0] = message;
}

tilia::utils::Tilia_Exception::Tilia_Exception(const char* message, const size_t& line, const std::string& file)
	:
	m_messages{ 1 },
	m_origin_line{ line },
	m_origin_file{ file }
{
	m_messages[0] = message;
}

tilia::utils::Tilia_Exception::Tilia_Exception(const std::stringstream& message, const size_t& line, const std::string& file)
	:
	m_messages{ 1 },
	m_origin_line{ line },
	m_origin_file{ file }
{
	m_messages[0] = message.str();
}

void tilia::utils::Tilia_Exception::Add_Message(const std::string& message)
{
	m_messages.push_back(message);
}

void tilia::utils::Tilia_Exception::Add_Message(const char* message)
{
	m_messages.push_back(message);
}

void tilia::utils::Tilia_Exception::Add_Message(const std::stringstream& message)
{
	m_messages.push_back(message.str());
}
