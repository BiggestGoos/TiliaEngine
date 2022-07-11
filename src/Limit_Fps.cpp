#include "headers/Limit_Fps.h"
#include <thread>

void S2D::utils::Sleep_Program(const unsigned int& target_frames) {

    next_frame += std::chrono::milliseconds(lround(1000.0f / target_frames));

    std::this_thread::sleep_until(next_frame);

}
