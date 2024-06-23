#include <iostream>
#include <vlc/vlc.h>
#include <X11/Xlib.h>
#include "CLI_menu.h"
#include "control.h"
#include "playback_time.h"

void playMedia(libvlc_instance_t* initPlayer, const std::string& selectedFile) {
    libvlc_media_t *playableMedia = libvlc_media_new_path(initPlayer, selectedFile.c_str());
    if (!playableMedia) {
        std::cerr << "Please select file to open!" << std::endl;
        libvlc_release(initPlayer);
        return;
    }

    libvlc_media_player_t *mediaPlayer = libvlc_media_player_new_from_media(playableMedia);
    if (!mediaPlayer) {
        std::cerr << "Failed to start player" << std::endl;
        libvlc_media_release(playableMedia);
        libvlc_release(initPlayer);
        return;
    }

    Display *display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Failed to open X display." << std::endl;
        libvlc_media_player_release(mediaPlayer);
        libvlc_release(initPlayer);
        return;
    }

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);
    Colormap cMap = DefaultColormap(display, screen);
    XColor greenColor;

    XParseColor(display, cMap, "#a7ff83", &greenColor);
    XAllocColor(display, cMap, &greenColor);

    XSetWindowAttributes windowAttributes;
    windowAttributes.background_pixel = greenColor.pixel;

    Window win = XCreateWindow(display, root, 10, 10, 650, 650, 1, DefaultDepth(display, screen),
                               InputOutput, DefaultVisual(display, screen), CWBackPixel, &windowAttributes);

    XStoreName(display, win, "CMP");
    XMapWindow(display, win);
    XFlush(display);

    libvlc_media_player_set_xwindow(mediaPlayer, win);

    libvlc_media_release(playableMedia);
    libvlc_media_player_play(mediaPlayer);

    std::atomic<bool>run(true);
    std::thread updateThread(playbackTime, mediaPlayer, std::ref(run));

    std::cout << "\nMedia is playing..." << std::endl;

    char mediaControl;
    float currentSpeed = 1.0;

    while (mediaPlayer) {
        std::cin >> mediaControl;
        switch (mediaControl) {
            case 'p': libvlc_media_player_pause(mediaPlayer); break;
            case 'm': {
                bool muted = libvlc_audio_get_mute(mediaPlayer);
                libvlc_audio_set_mute(mediaPlayer, !muted);
                break;
            }
            case '=': {
                int currentVolume = libvlc_audio_get_volume(mediaPlayer);
                libvlc_audio_set_volume(mediaPlayer, currentVolume + 5);
                break;
            }
            case '-': {
                int currentVolume = libvlc_audio_get_volume(mediaPlayer);
                libvlc_audio_set_volume(mediaPlayer, currentVolume - 5);
                break;
            }
            case 'C': currentSpeed += 1.5; libvlc_media_player_set_rate(mediaPlayer, currentSpeed); break;
            case 'D': currentSpeed -= 1.5; libvlc_media_player_set_rate(mediaPlayer, currentSpeed); break;
            case 'q': std::cout << "\nExiting..." << std::endl; goto quit;
        }
    }

quit:
    run = false;
    updateThread.join();
    libvlc_media_player_stop(mediaPlayer);
    libvlc_media_player_release(mediaPlayer);
    libvlc_release(initPlayer);
}

void initializeNonCanonicalMode() {
    setNonCanonicalMode(true);
}

void finalizeNonCanonicalMode() {
    setNonCanonicalMode(false);
}

std::string selectFile() {
    std::string selectedFile = browseFile("/home");
    if (selectedFile.empty()) {
        std::cerr << "Nothing to select" << std::endl;
    }
    return selectedFile;
}

libvlc_instance_t* initializeVLC() {
    libvlc_instance_t *initPlayer = libvlc_new(0, nullptr);
    if (!initPlayer) {
        std::cerr << "Init failed! Retry" << std::endl;
    }
    return initPlayer;
}

void runMediaPlayer(const std::string& selectedFile) {
    libvlc_instance_t* initPlayer = initializeVLC();
    if (!initPlayer) {
        return;
    }

    playMedia(initPlayer, selectedFile);
    libvlc_release(initPlayer);
}

int main(int argc, char *argv[]) {
    initializeNonCanonicalMode();

    std::string selectedFile = selectFile();
    if (!selectedFile.empty()) {
        runMediaPlayer(selectedFile);
    }

    finalizeNonCanonicalMode();
    return 0;
}