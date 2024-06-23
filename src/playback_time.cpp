#include "playback_time.h"
#include <iomanip>

void playbackTime(libvlc_media_player_t *mediaPlayer, std::atomic<bool> &run)
{
    while (run)
    {
        libvlc_time_t current_time = libvlc_media_player_get_time(mediaPlayer);

        int seconds = current_time / 1000;
        int minutes = seconds / 60;
        seconds = seconds % 60;

        std::cout << "\rCurrent time: "
                  << std::setw(2) << std::setfill('0') << minutes << ":"
                  << std::setw(2) << std::setfill('0') << seconds
                  << std::flush;

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}
