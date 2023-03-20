// Tilia
#include "Tilia_Exception.hpp"

#if TILIA_UNIT_TESTS == 1

// Vendor
#include "vendor/Catch2/Catch2.hpp"

#define INT_VALUE 123
#define FLOAT_VALUE 0.15f
#define DOUBLE_VALUE 3.14
#define BOOL_VALUE true
#define STRING_VALUE "Hello World!"

void tilia::utils::Tilia_Exception::Test()
{

	const std::string fake_file_0{ "Z:/Test/Some_File.cpp" };
	const std::size_t fake_line_0{ 132 };
	const std::string fake_error_message_0{ "Some error message about stuff and whatever..." };

	// Test for constructing with message

	Exception_Data e_d_0{ fake_file_0, fake_line_0, fake_error_message_0, INT_VALUE, FLOAT_VALUE, 
		DOUBLE_VALUE, BOOL_VALUE, STRING_VALUE };

	std::stringstream additional_values_0{};
	additional_values_0 << INT_VALUE << FLOAT_VALUE << DOUBLE_VALUE << BOOL_VALUE << STRING_VALUE;

	REQUIRE(e_d_0.Get_File() == fake_file_0);
	REQUIRE(e_d_0.Get_Line() == fake_line_0);
	REQUIRE(e_d_0.Get_Location() == std::pair{ fake_file_0, fake_line_0 });
	REQUIRE(e_d_0.Get_Message() == fake_error_message_0 + additional_values_0.str());

	// Test for copy constructing

	Exception_Data e_d_1{ e_d_0 };

	REQUIRE(e_d_1.Get_File() == fake_file_0);
	REQUIRE(e_d_1.Get_Line() == fake_line_0);
	REQUIRE(e_d_1.Get_Location() == std::pair{ fake_file_0, fake_line_0 });
	REQUIRE(e_d_1.Get_Message() == fake_error_message_0 + additional_values_0.str());

	REQUIRE(e_d_0 == e_d_1);

	REQUIRE_FALSE(e_d_0 != e_d_1);

	// Test for move constructing

	Exception_Data e_d_2{ std::move(e_d_0) };

	REQUIRE(e_d_2.Get_File() == fake_file_0);
	REQUIRE(e_d_2.Get_Line() == fake_line_0);
	REQUIRE(e_d_2.Get_Location() == std::pair{ fake_file_0, fake_line_0 });
	REQUIRE(e_d_2.Get_Message() == fake_error_message_0 + additional_values_0.str());

	REQUIRE(e_d_1 == e_d_2);

	// Test for construction with just file and line

	Exception_Data e_d_3{ fake_file_0, fake_line_0 };

	REQUIRE(e_d_3.Get_File() == fake_file_0);
	REQUIRE(e_d_3.Get_Line() == fake_line_0);
	REQUIRE(e_d_3.Get_Location() == std::pair{ fake_file_0, fake_line_0 });

	// Test for setting message with function

	e_d_3.Set_Message(fake_error_message_0);

	REQUIRE(e_d_3.Get_Message() == fake_error_message_0);

	// Test for adding to message with function

	e_d_3.Append_Message(INT_VALUE, FLOAT_VALUE, DOUBLE_VALUE, BOOL_VALUE, STRING_VALUE);

	REQUIRE(e_d_3 == e_d_2);

	// Test for constructing with exception data

	Tilia_Exception t_e_0{ e_d_1 };

	REQUIRE(t_e_0.Get_Message(0) == e_d_1);

	// Test for move constructing with exception data

	Tilia_Exception t_e_1{ std::move(e_d_1) };

	REQUIRE(t_e_0.Get_Message(0) == t_e_1.Get_Message(0));
	
	REQUIRE(t_e_0 == t_e_1);

	REQUIRE_FALSE(t_e_0 != t_e_1);

	// Test for copy constructing

	Tilia_Exception t_e_2{ t_e_0 };

	REQUIRE(t_e_2 == t_e_0);

	// Test for move constructing

	Tilia_Exception t_e_3{ std::move(t_e_0) };

	REQUIRE(t_e_3 == t_e_2);

	const std::string fake_file_1{ "G:/Test/Some_Other_File.cpp" };
	const std::size_t fake_line_1{ 628 };
	const std::string fake_error_message_1{ "Some other error message about other stuff and not whatever..." };

	// Test for adding multiple messages

	Exception_Data e_d_4{ fake_file_1, fake_line_1, fake_error_message_1 };

	Tilia_Exception t_e_4{ e_d_2 };

	REQUIRE(t_e_4.Get_Count() == 1);

	t_e_4.Add_Message(e_d_4);

	REQUIRE(t_e_4.Get_Count() == 2);

	REQUIRE(t_e_4 != t_e_3);

	t_e_4.Add_Message(std::move(e_d_4));

	REQUIRE(t_e_4.Get_Count() == 3);

	REQUIRE(t_e_4.Get_Message(1) == t_e_4.Get_Message(2));

	// Test for throwing and catching as general exception

	try
	{
		throw t_e_4;
	}
	catch (const std::exception& e)
	{
		REQUIRE(std::strcmp(e.what(), t_e_4.what()) == 0);
	}

}

#endif // TILIA_UNIT_TESTS == 1
