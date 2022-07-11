
#ifndef S2D_LIMIT_FPS_H
#define S2D_LIMIT_FPS_H

#include <iostream>
#include <chrono>

namespace S2D {

	namespace utils {

#ifndef S2D_CLOCK
#define S2D_CLOCK
		using clock = std::chrono::steady_clock;
#endif

#ifndef S2D_NEXT_FRAME
#define S2D_NEXT_FRAME
		static auto next_frame = clock::now();
#endif

		void Sleep_Program(const unsigned int& target_frames);

	}

}

#endif
