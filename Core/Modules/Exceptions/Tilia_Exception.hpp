/**************************************************************************************************
 * @file   Tilia_Exception.hpp
 * 
 * @brief  Holds a custom exception class deriving from the <stdexcept> exception class.
 * 
 * @define TILIA_TILIA_EXCEPTION_HPP
 * 
 * Standard:
 * <stdexcept>
 * <string>
 * <vector>
 * <sstream>
 * 
 * @author Gustav Fagerlind
 * @date   10/07/2022
 *************************************************************************************************/

#ifndef TILIA_TILIA_EXCEPTION_HPP
#define TILIA_TILIA_EXCEPTION_HPP

// Standard
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>

/**
 * @brief Small macro which can be used in the construcor of the Tilia_Exception class
 */
#define LOCATION static_cast<std::size_t>(__LINE__), __FILE__

namespace tilia {

	namespace utils {

		/**
		 * @brief Custom exception class deriving from the <stdexcept> exception class. Holds a
		 *		  vector of messages which can be added to while the exception is being passed
		 *		  along. Also holds the origin line and file which will point to the place where
		 *		  the exception was first thrown.
		 */
		class Tilia_Exception 
			: public std::exception 
		{
		private:

			/**
			 * @brief Checks wheter the message holds a message code at the given index.
			 */
			inline bool Is_Value_Code(const std::size_t& index, const std::string& message) noexcept {
				return (index < message.size() - 1 && message[index] == '%' && message[index + 1] == 'v');
			}

			/**
			 * @brief Recursively adds values to a stored message. It gets the value by a returned
			 *		  lambda function which adds the value and then calls this function again.
			 * 
			 * @param message - The message which will be checked for a place to add the value
			 *					given in the lambda function.
			 * 
			 * @return A lambda function which takes in a value, adds the value to the string at 
			 *		   the captured index in the message with the captured message index. After 
			 *		   this it just calls the function again and it does the same thing again until
			 *		   you stop calling the returned lambda function.
			 */
			auto Print_To_String(const std::string& message) noexcept
			{

				static constexpr std::size_t value_string_size{ 2 };

				const std::size_t message_id{ m_messages.size() - 1 };

				const std::size_t message_len{ message.size() };

				std::size_t index{};
				for (index = 0; index < message_len; ++index)
				{

					if (Is_Value_Code(index, message)) {

						m_messages[message_id].erase(index, value_string_size);
						break;

					}

				}

				return [message_id, index, this](const auto& value) {
					std::stringstream ss{};
					ss << value;
					m_messages[message_id].insert(index, ss.str());
					return Print_To_String(m_messages[message_id]);
				};

			}

		public:

			/**
			 * @brief Default copy constructor.
			 * 
			 * @param other - The Tilia_Exception to copy.
			 */
			Tilia_Exception(const Tilia_Exception& other) = default;

			/**
			 * @brief Sets the line and file that the Tilia_Exception originates from.
			 * 
			 * @param line - The line where exception was thrown.
			 * @param file - The file where exception was thrown.
			 */
			explicit Tilia_Exception(const std::size_t& line, const std::string& file) noexcept;

			/**
			 * @brief Adds the message to the message vector and sets the origin line and file.
			 * 
			 * @param message - The message to add.
			 * @param line - The line where exception was thrown.
			 * @param file - The file where exception was thrown.
			 */
			explicit Tilia_Exception(const std::string& message, const std::size_t& line, const std::string& file) noexcept;

			/**
			 * @brief Adds a message to the Tilia_Exception. If the message contains message 
			 *		  commands and the returned lambda is called with a value then that value
			 *	      will be inserted in the string at the first message command. That lambda
			 *		  function will return a lambda itself and will recursivley replace message
			 *		  commands with the given values.
			 * 
			 * @param message - The message to add. Message command: %v
			 * 
			 * @return A lambda function which can be called to insert values into the string at a
			 *		   specified place.
			 */
			auto Add_Message(const std::string& message) noexcept {

				m_messages.push_back({ message });

				return Print_To_String(message);

			}

			/**
			 * @brief Gets all of the messages concatenated into one string.
			 * 
			 * @return All of the messages in one string.
			 */
			virtual const char* what() const noexcept override {
				static std::string ret{ "" };
				for (const auto& w : m_messages)
				{
					ret += w + '\n';
				}
				return ret.c_str();
			}

			/**
			 * @brief Gets the vector of all the messages.
			 * 
			 * @return All of the messages added to the exception.
			 */
			std::vector<std::string> Get_Messages() const noexcept { return m_messages; }

			/**
			 * @brief Gets the line the exception originates from.
			 * 
			 * @return The origin line.
			 */
			std::size_t Get_Origin_Line() const noexcept { return m_origin_line; }

			/**
			 * @brief Gets the file the exception originates from.
			 *
			 * @return The origin file.
			 */
			std::string Get_Origin_File() const noexcept { return m_origin_file; }

		private:

			std::vector<std::string> m_messages{};

			std::size_t m_origin_line{};
			std::string m_origin_file{};

		}; // Tilia_Exception

	} // utils

} // tilia

#endif // TILIA_TILIA_EXCEPTION_HPP