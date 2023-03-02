// Tilia
#include "Tilia_Exception.hpp"

#if TILIA_UNIT_TESTS == 1

// Vendor
#include "vendor/Catch2/Catch2.hpp"

void tilia::utils::Tilia_Exception::Test()
{

	const std::string fake_file{ "Z:/Test/Some_File.cpp" };
	const std::size_t fake_line{ 132 };
	const std::string fake_error_message{ "Some error message about stuff and whatever..." };

	Exception_Data e_d{ fake_file, fake_line, fake_error_message };

	Tilia_Exception t_e{ e_d };

	auto message = t_e.Get_Message(0);

	REQUIRE(e_d == message);

	REQUIRE(e_d.Get_File() == fake_file);
	REQUIRE(message.Get_File() == fake_file);

	REQUIRE(e_d.Get_Line() == fake_line);
	REQUIRE(message.Get_Line() == fake_line);

	REQUIRE(e_d.Get_Message() == fake_error_message);
	REQUIRE(message.Get_Message() == fake_error_message);

	static auto potential_message{ [](const auto& message)
	{
		const auto message_length{ message.size() };
		if (message_length > 0)
			return message.c_str();
		return "Message is empty";
	} };

	std::stringstream result_of_what{};

	result_of_what << "Message #" << 1 << ":\n" << "File: " << message.m_file << " : Line: " << message.m_line << "\nMessage:\n" <<
		potential_message(message.m_message) << "\n\n";

	REQUIRE(t_e.what() == result_of_what.str());

}

#endif // TILIA_UNIT_TESTS == 1
