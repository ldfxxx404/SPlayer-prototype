#include <iostream>
#include "control.h"
#include <vlc/vlc.h>
#include <X11/Xlib.h>
#include "CLI_menu.h"

void playMedia(libvlc_instance_t* initPlayer, const std::string& selectedFile);

int main(int argc, char *argv[]) {
    setNonCanonicalMode(true);

    std::string selectedFile = browseFile(".");

    if (selectedFile.empty()) {
        std::cerr << "Nothing to select" << std::endl;
        return -1;
    }

    libvlc_instance_t *initPlayer = libvlc_new(0, nullptr);
    if (!initPlayer) {
        std::cerr << "Init failed! Retry" << std::endl;
        return -1;
    }

    playMedia(initPlayer, selectedFile);

    setNonCanonicalMode(false);
    return 0;
}

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
    XColor greenColor;
    Colormap cMap = DefaultColormap(display, screen);
    XParseColor(display, cMap, "#a7ff83", &greenColor);
    XAllocColor(display, cMap, &greenColor);
    XSetWindowAttributes windowAttributes;
    windowAttributes.background_pixel = greenColor.pixel;
    Window win = XCreateWindow(display, root, 10, 10, 650, 650, 1, DefaultDepth(display, screen),
                               InputOutput, DefaultVisual(display, screen), CWBackPixel, &windowAttributes);

    libvlc_media_player_set_xwindow(mediaPlayer, win);
    XStoreName(display, win, "SPlayer");
    XMapWindow(display, win);
    XFlush(display);

    libvlc_media_release(playableMedia);
    libvlc_media_player_play(mediaPlayer);

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
            case 'q': std::cout << "Exiting..." << std::endl; goto quit;
            default: break;
        }
    }

quit:
    libvlc_media_player_stop(mediaPlayer);
    libvlc_media_player_release(mediaPlayer);
    libvlc_release(initPlayer);
}
