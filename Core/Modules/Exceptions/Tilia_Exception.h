/*****************************************************************//**
 * @file   Tilia_Exception.h
 * @brief  An exception class deriving from @include <stdexcept> 
 * @exception
 * 
 * @define TILIA_TILIA_EXCEPTION_H
 * 
 * Standard:
 * @include <stdexcept>
 * @include <string>
 * @include <vector>
 * @include <sstream>
 * 
 * @author Gustav Fagerlind
 * @date   10/07/2022
 *********************************************************************/

#ifndef TILIA_TILIA_EXCEPTION_H
#define TILIA_TILIA_EXCEPTION_H

// Standard
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>

#define LOCATION static_cast<size_t>(__LINE__), __FILE__

namespace tilia {

	namespace utils {

		/**
		 * @brief Custom exception class. Holds a vector of messages which can be added to while 
		 * the exception is being passed along. Also holds the origin line and file which will 
		 * point the the place where the first exception was thrown of this exception.
		 */
		class Tilia_Exception 
			: public std::exception 
		{
		public:

			/**
			 * @brief Adds the message to the message vector and sets the origin line and file.
			 * 
			 * @param message - The message to add.
			 * @param line - The line where exception was thrown.
			 * @param file - The file where exception was thrown.
			 */
			explicit Tilia_Exception(const std::string& message, const size_t& line, const std::string& file);
			
			/**
			 * @brief Adds the message to the message vector and sets the origin line and file.
			 *
			 * @param message - The message to add.
			 * @param line - The line where exception was thrown.
			 * @param file - The file where exception was thrown.
			 */
			explicit Tilia_Exception(const char* message, const size_t& line, const std::string& file);

			/**
			 * @brief Adds the message to the message vector and sets the origin line and file.
			 *
			 * @param message - The message to add.
			 * @param line - The line where exception was thrown.
			 * @param file - The file where exception was thrown.
			 */
			explicit Tilia_Exception(const std::stringstream& message, const size_t& line, const std::string& file);
			
			/**
			 * @brief Adds the message to the message vector.
			 * 
			 * @param message - The message to add.
			 */
			void Add_Message(const std::string& message);

			/**
			 * @brief Adds the message to the message vector.
			 *
			 * @param message - The message to add.
			 */
			void Add_Message(const char* message);

			/**
			 * @brief Adds the message to the message vector.
			 *
			 * @param message - The message to add.
			 */
			void Add_Message(const std::stringstream& message);

			/**
			 * @brief Gets all of the messages concatenated togheter into one string.
			 * 
			 * @return All of the messages in one string.
			 */
			virtual const char* what() const override {
				std::string ret{};
				for (const auto& w : m_messages)
				{
					ret += w;
				}
				return ret.c_str();
			}

			/**
			 * @brief Gets the vector of messages.
			 * 
			 * @return All of the messages added to the exception.
			 */
			const std::vector<std::string>& Get_Messages() const { return m_messages; }

			/**
			 * @brief Gets the line the exception originates from.
			 * 
			 * @return The origin line.
			 */
			const size_t& Get_Origin_Line() const { return m_origin_line; }

			/**
			 * @brief Gets the file the exception originates from.
			 *
			 * @return The origin file.
			 */
			const std::string& Get_Origin_File() const { return m_origin_file; }

		private:

			std::vector<std::string> m_messages{}; // The messages of the exception.

			size_t m_origin_line{}; // The line the exception originates from.
			std::string m_origin_file{}; // The file the exception originates from.

		};

	}

}

#endif