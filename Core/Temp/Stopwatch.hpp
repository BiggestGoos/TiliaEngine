/*****************************************************************//**
 * @file   Stopwatch.h
 * @brief  Used to profile the time it takes between specified points.
 * 
 * @define TILIA_STOPWATCH_H
 * 
 * Standard:
 * @include <string>
 * @include <cstdint>
 * @include <utility>
 * 
 * @author Gustav Fagerlind
 * @date   30/06/2022
 *********************************************************************/

#ifndef TILIA_STOPWATCH_H
#define TILIA_STOPWATCH_H

// Standard
#include <string>
#include <cstdint>
#include <utility>

namespace tilia {

	namespace utils {

		/**
		 * @brief The different modes to use for the () operator for the @Stopwatch class.
		 */
		enum class Mode
		{
			Start = 0, // Starts the stopwatch.
			Stop = 1,  // Stops the stopwatch.
		};

		/**
		 * @brief Functor class which will take the time it takes between calling its 
		 * overloaded () operator with the start and stop modes.
		 */
		class Stopwatch {
		public:

			/**
			 * @brief Depending on the given mode it will get some time values and then print 
			 * it along with a given message only if the given bool is true.
			 * 
			 * @param mode - The mode to use.
			 * @param message - The message to print.
			 * @param should_print - Wheter or not to print anything.
			 * 
			 * @return - The active time of the stopwatch.
			 */
			int64_t operator()(const Mode& mode, const std::string& message, bool should_print = true);

		private:

			/**
			 * @brief Gets the current passed time of the opened window along with the current 
			 * passed time of the timer so far which will be zero. It also stores the current 
			 * window time.
			 * 
			 * @return The current total passed time along with the stopwatches passed time.
			 */
			std::pair<int64_t, int64_t> Start();

			/**
			 * @brief Gets the current passed time of the opened window along with the current
			 * passed time of the timer so far which will be current passed time of the opened 
			 * window minus the previosly stored start time.
			 *
			 * @return The current total passed time along with the stopwatches passed time.
			 */
			std::pair<int64_t, int64_t> Stop();

			// The passed time relative to the window creation when the () was called with the
			// starting mode.
			int64_t m_start_time{};

		};

	}

}

#endif