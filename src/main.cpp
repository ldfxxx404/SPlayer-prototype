#include <iostream>
#include <vlc/vlc.h>
#include <X11/Xlib.h>
#include <atomic>
#include <thread>
#include <map>
#include <functional>
#include "CLI_menu.h"
#include "control.h"
#include "playback_time.h"

class VLCInstance {

public:

    VLCInstance() {
        instance_ = libvlc_new(0, nullptr);

        if (!instance_) {
            std::cerr << "Failed to initialize VLC instance" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    ~VLCInstance() {
        if (instance_) {
            libvlc_release(instance_);
        }
    }

    libvlc_instance_t* getInstance() const {
        return instance_;
    }

private:
    libvlc_instance_t* instance_;
};

class X11Window {

public:

    X11Window(libvlc_media_player_t* mediaPlayer) : display_(nullptr), window_(0) {
        initialize(mediaPlayer);
    }

    ~X11Window() {
        if (display_) {
            XCloseDisplay(display_);
        }
    }

private:

    void initialize(libvlc_media_player_t* mediaPlayer) {
        display_ = XOpenDisplay(nullptr);

        if (!display_) {
            std::cerr << "Failed to open X display." << std::endl;
            exit(EXIT_FAILURE);
        }

        int screenNumber = DefaultScreen(display_);
        Window rootWindow = RootWindow(display_, screenNumber);
        XColor windowBackgroundColor;
        Colormap colormap = DefaultColormap(display_, screenNumber);

        XParseColor(display_, colormap, "#a7ff83", &windowBackgroundColor);
        XAllocColor(display_, colormap, &windowBackgroundColor);

        XSetWindowAttributes windowAttributes;
        windowAttributes.background_pixel = windowBackgroundColor.pixel;

        window_ = XCreateWindow(display_, rootWindow, 10, 10, 650, 650, 1,
                                DefaultDepth(display_, screenNumber), InputOutput,
                                DefaultVisual(display_, screenNumber), CWBackPixel, &windowAttributes);

        libvlc_media_player_set_xwindow(mediaPlayer, window_);
        XStoreName(display_, window_, "CMP");
        XMapWindow(display_, window_);
        XFlush(display_);
    }

    Display* display_;
    Window window_;
};

class MediaPlayer {

public:

    MediaPlayer(libvlc_instance_t* vlcInstance, const std::string& mediaPath, bool isUrl = false)
        :mediaPath_(mediaPath), mediaPlayer_(nullptr), playbackSpeed_(1.0) {
        initialize(vlcInstance, mediaPath, isUrl);
        x11Window_ = std::make_unique<X11Window>(mediaPlayer_);
    }

    ~MediaPlayer() {
        if (playbackTimeThread_.joinable()) {
            playbackTimeThread_.join();
        }

        if (mediaPlayer_) {
            libvlc_media_player_stop(mediaPlayer_);
            libvlc_media_player_release(mediaPlayer_);
        }
    }

    void startPlayback() {
        libvlc_media_player_play(mediaPlayer_);
        playbackTimeThread_ = std::thread(playbackTime, mediaPlayer_, std::ref(isRunning_));
    }

    void pauseOrResume() {
        libvlc_media_player_pause(mediaPlayer_);
    }

    void muteOrUnmute() {
        bool isMuted = libvlc_audio_get_mute(mediaPlayer_);
        libvlc_audio_set_mute(mediaPlayer_, !isMuted);
    }

    void increaseVolume() {
        int currentVolume = libvlc_audio_get_volume(mediaPlayer_);
        libvlc_audio_set_volume(mediaPlayer_, currentVolume + 5);
    }

    void decreaseVolume() {
        int currentVolume = libvlc_audio_get_volume(mediaPlayer_);
        libvlc_audio_set_volume(mediaPlayer_, currentVolume - 5);
    }

    void increaseSpeed() {
        playbackSpeed_ += 1.5;
        libvlc_media_player_set_rate(mediaPlayer_, playbackSpeed_);
    }

    void decreaseSpeed() {
        playbackSpeed_ -= 1.5;
        libvlc_media_player_set_rate(mediaPlayer_, playbackSpeed_);
    }

    bool isRunning() const {
        return isRunning_;
    }

    void stop() {
        isRunning_ = false;
    }

    std::string getMediaTitle() const {
        return mediaPath_;
    }

private:

    void initialize(libvlc_instance_t* vlcInstance, const std::string& mediaPath, bool isUrl) {
        libvlc_media_t* media;

        if (isUrl) {
            media = libvlc_media_new_location(vlcInstance, mediaPath.c_str());
        } else {
            media = libvlc_media_new_path(vlcInstance, mediaPath.c_str());
        }
        
        if (!media) {
            std::cerr << "Failed to create media object!" << std::endl;
            exit(EXIT_FAILURE);
        }

        mediaPlayer_ = libvlc_media_player_new_from_media(media);
        libvlc_media_release(media);
        
        if (!mediaPlayer_) {
            std::cerr << "Failed to create media player!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    std::string mediaPath_;
    libvlc_media_player_t* mediaPlayer_;
    std::unique_ptr<X11Window> x11Window_;
    std::atomic<bool> isRunning_{true};
    std::thread playbackTimeThread_;
    float playbackSpeed_;
};

class UserInputHandler {

public:

    UserInputHandler(MediaPlayer& mediaPlayer) : mediaPlayer_(mediaPlayer) {

        userCommandMap_ = {
            {'p', [&mediaPlayer]() { mediaPlayer.pauseOrResume(); }},
            {'m', [&mediaPlayer]() { mediaPlayer.muteOrUnmute(); }},
            {'=', [&mediaPlayer]() { mediaPlayer.increaseVolume(); }},
            {'-', [&mediaPlayer]() { mediaPlayer.decreaseVolume(); }},
            {'C', [&mediaPlayer]() { mediaPlayer.increaseSpeed(); }},
            {'D', [&mediaPlayer]() { mediaPlayer.decreaseSpeed(); }},
            {'q', [&mediaPlayer]() { mediaPlayer.stop(); }}
        };
    }

    void processUserInput() {

        char userCommand;
        std::cout << "\nMedia is playing..." << std::endl;
        
        while (mediaPlayer_.isRunning()) {
            std::cin >> userCommand;
            handleUserCommand(userCommand);

            if (userCommand == 'q') {
                std::cout << "\nExiting..." << std::endl;
                break;
            }
        }
    }

private:

    void handleUserCommand(char userCommand) {

        auto commandHandler = userCommandMap_.find(userCommand);
        
        if (commandHandler != userCommandMap_.end()) {
            commandHandler->second();
        }
    }

    MediaPlayer& mediaPlayer_;
    std::map<char, std::function<void()>> userCommandMap_;
};

// Класс для управления неконаноническим режимом терминала
class NonCanonicalMode {

public:

    static void enable() {
        setNonCanonicalMode(true);
    }

    static void disable() {
        setNonCanonicalMode(false);
    }
};

class FileBrowser {

public:

    static std::string browse() {

        std::string mediaFilePath = browseFile("/home");

        if (mediaFilePath.empty()) {
            std::cerr << "No file selected. Exit" << std::endl;
        }

        return mediaFilePath;
    }
};

// Основная функция программы
int main(int argc, char *argv[]) {

    NonCanonicalMode::enable();

    while (true)
    {
        std::string mediaPath = FileBrowser::browse();

        if (mediaPath.empty()) {
            break;
        }
    
        bool isUrl = mediaPath.find("http://") == 0 || mediaPath.find("https://") == 0;

        VLCInstance vlcInstance;
        MediaPlayer mediaPlayer(vlcInstance.getInstance(), mediaPath, isUrl);
        UserInputHandler userInputHandler(mediaPlayer);

        std::cout << "\n========================" << std::endl;
        std::cout << "\nNow playing: " << mediaPlayer.getMediaTitle() << std::endl;

        mediaPlayer.startPlayback();
        userInputHandler.processUserInput();

        if (!mediaPlayer.isRunning()) {
            std::cout << "Returning to file browser menu..." << std::endl;
            NonCanonicalMode::enable();
        }
    }

    NonCanonicalMode::disable();
    return 0;
}
