#include <iostream>
#include "control.h"
#include <vlc/vlc.h>
#include <X11/Xlib.h>

int main(int argc, char *argv[])
{

    setNonCanonicalMode(true);

    /*Init libVLC*/
    libvlc_instance_t *initPlayer;

    /*Create playback for file*/
    libvlc_media_t *playableMedia;

    /*Create player*/
    libvlc_media_player_t *mediaPlayer;

    /*libvlc_instance_t *initPlayer;*/
    initPlayer = libvlc_new(0, nullptr);

    if (!initPlayer)
    {
        std::cerr << "Init failed! Retry" << std::endl;

        return -1;
    }

    /*Set playable media path*/
    playableMedia = libvlc_media_new_path(initPlayer, argv[1]);

    if (!playableMedia)
    {
        std::cerr << "Please select file to open!" << std::endl;

        /*If error terminate media*/
        libvlc_release(initPlayer);

        return -1;
    }

    mediaPlayer = libvlc_media_player_new_from_media(playableMedia);

    if (!playableMedia)
    {
        std::cerr << "Failed to start player" << std::endl;

        libvlc_media_release(playableMedia);
        libvlc_release(initPlayer);

        return -1;
    }

    /*Init X11 window*/
    Display *display = XOpenDisplay(nullptr);
    if (!display)
    {
        std::cerr << "Failed to open X display." << std::endl;
        libvlc_media_player_release(mediaPlayer);
        libvlc_release(initPlayer);
        return -1;
    }

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    /*Set color*/
    XColor greenColor;
    Colormap cMap;
    cMap = DefaultColormap(display, screen);
    XParseColor(display, cMap, "#a7ff83", &greenColor);
    XAllocColor(display, cMap, &greenColor);
    XSetWindowAttributes windowAtributes;
    windowAtributes.background_pixel = greenColor.pixel;

    /*Set window size, etc.*/
    Window win = XCreateWindow(display, root, 10, 10, 650, 650, 1, DefaultDepth(display, screen),
                               InputOutput, DefaultVisual(display, screen), CWBackPixel, &windowAtributes);

    /*Setting the X11 window as the video output for the media player*/
    libvlc_media_player_set_xwindow(mediaPlayer, win);

    /*Set titile*/
    XStoreName(display, win, "SPlayer");
    XMapWindow(display, win);
    XFlush(display);

    /*Starting playback file*/
    libvlc_media_release(playableMedia);

    /*Starting player*/
    libvlc_media_player_play(mediaPlayer);

    std::cout << "\nMedia is playing..." << std::endl;

    /*Pause control, volume control, etc.*/
    char mediaControl;
    char currentSpeed = 1.0;

    while (mediaPlayer)
    {

        std::cin >> mediaControl;

        switch (mediaControl)
        {

        case 'p':
        {
            libvlc_media_player_pause(mediaPlayer);
            break;
        }

        case 'm':
        {
            bool muted = libvlc_audio_get_mute(mediaPlayer);
            libvlc_audio_set_mute(mediaPlayer, !muted);
            break;
        }
        case '=':
        {
            libvlc_audio_set_volume(mediaPlayer, libvlc_audio_get_volume(mediaPlayer) + 10);
            break;
        }
        case '-':
        {
            libvlc_audio_set_volume(mediaPlayer, libvlc_audio_get_volume(mediaPlayer) - 10);
            break;
        }
        case 'C': /*right arrow key*/
        {
            libvlc_media_player_set_rate(mediaPlayer, currentSpeed + 1.5);
            break;
        }
        case 'D': /*left arrow key*/
        {
            libvlc_media_player_set_rate(mediaPlayer, currentSpeed);
            break;
        }
        case 'q':
        {
            std::cout << "Exiting..." << std::endl;
            goto quit;
            break;
        }
        }
    }

/*END*/
quit:
    /*Stopping media playback*/
    libvlc_media_player_stop(mediaPlayer);

    /*Freeing up resources occupied by the media player*/
    libvlc_media_player_release(mediaPlayer);

    /*Release resources occupied by libVLC*/
    libvlc_release(initPlayer);

    setNonCanonicalMode(false);

    return 0;
}
