#ifndef PLAYBACK_TIME_H
#define PLAYBACK_TIME_H

#include <iostream>
#include <vlc/vlc.h>
#include <thread>
#include <chrono>
#include <atomic>

void playbackTime(libvlc_media_player_t *mediaPlayer, std::atomic<bool> &run);

#endif // PLAYBACK_TIME_H