/**************************************************************************************************
 * @file   Exception_Handler.hpp
 * 
 * @brief  Holds a singleton class which can recieve exceptions from any given thread and will, 
 *		   when told, output the exceptions to the console.
 * 
 * @author Gustav Fagerlind
 * @date   04/03/2023
 *************************************************************************************************/

#ifndef TILIA_EXCEPTION_HANDLER_HPP
#define TILIA_EXCEPTION_HANDLER_HPP

// Standard
#include <vector>
#include <mutex>

// Tilia
#include "Core/Values/Directories.hpp"
#include TILIA_TILIA_EXCEPTION_INCLUDE
#include TILIA_CONSTANTS_INCLUDE

namespace tilia
{
	namespace utils
	{

		/**
		 * @brief Exception handling singleton class. Can be used from any thread. Should be 
		 * completely thread safe.
		 */
		class Exception_Handler
		{
		public:

			/**
			 * @brief First time it is called it will construct an instance of Exception_Handler. 
			 * A reference to this instance is returned to anywhere in the program.
			 *
			 * @return A reference to an instance of Exception_Handler.
			 */
			static Exception_Handler& Instance()
			{
				static Exception_Handler exception_handler{};
				return exception_handler;
			}

			/**
			 * @brief Adds an exception to be handled.
			 * 
			 * @param exception - The exception to be handled.
			 */
			void Throw(const Tilia_Exception& exception);
			/**
			 * @brief Adds an exception to be handled.
			 *
			 * @param exception - The exception to be handled.
			 */
			void Throw(Tilia_Exception&& exception);

			/**
			 * @brief Adds an exception to be handled.
			 *
			 * @param exception - The exception to be handled.
			 */
			void Throw(const std::exception& exception);
			/**
			 * @brief Adds an exception to be handled.
			 *
			 * @param exception - The exception to be handled.
			 */
			void Throw(std::exception&& exception);

			/**
			 * @brief Handles and then removes them the stored exceptions.
			 */
			void Update();

#if TILIA_UNIT_TESTS == 1

			/**
			 * @brief Unit test for Tilia_Exception.
			 */
			static void Test();

#endif // TILIA_UNIT_TESTS == 1

		private:

			Exception_Handler() = default;

			// The stored tilia exceptions
			std::vector<Tilia_Exception> m_tilia_exceptions{};
			// The stored general exceptions of type std::exception or others deriving from it
			std::vector<std::exception> m_exceptions{};
			// A mutex which is used for the whole class
			mutable std::mutex m_mutex{};

		public:

			// Exception_Handler shan't be copyable or moveable

			Exception_Handler(const Exception_Handler& other) = delete;
			Exception_Handler(Exception_Handler&& other) = delete;

			Exception_Handler& operator=(const Exception_Handler& other) = delete;
			Exception_Handler& operator=(Exception_Handler&& other) = delete;

		}; // Exception_Handler

	} // utils
} // tilia

#endif // TILIA_EXCEPTION_HANDLER_HPP
