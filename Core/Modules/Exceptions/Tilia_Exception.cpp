// Tilia
#include "Tilia_Exception.hpp"

tilia::utils::Tilia_Exception::Tilia_Exception(const std::size_t& line, const std::string& file) 
noexcept
	:
	m_origin_line{ line },
	m_origin_file{ file }
{
}

tilia::utils::Tilia_Exception::Tilia_Exception(const std::string& message, const std::size_t& line, 
	const std::string& file) noexcept
	:
	m_messages{ 1 },
	m_origin_line{ line },
	m_origin_file{ file }
{
	m_messages[0] = message;
}
