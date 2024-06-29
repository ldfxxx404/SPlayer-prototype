# ConsoleMediaPlayer

ConsoleMediaPlayer is a CLI-based media player that provides essential playback controls, speed control, and volume adjustment. The project aims to extend its functionality over time, introducing more features and improving the user experience.

## Features

### Current Capabilities:
1. **Playback Control**:
   - Play
   - Pause
   - Mute

2. **Speed Control**:
   - Increase playback speed (X2)

3. **Volume Control**:
   - Adjust volume level

### Planned Features:
- Additional playback controls
- Enhanced CLI interface
- Advanced media handling features

## Screenshots

### Preview
![Preview](screenshots/example.png)

### System Resource Usage
![System resource usage](screenshots/system_resource_usage.png)

## Building the Project

### Build Instructions

1. Clone the repository and navigate to the `src` directory:
    ```sh
    git clone <repository_url>
    cd <repository>/src/
    ```

2. Create a `build` directory and navigate into it:
    ```sh
    mkdir build
    cd build
    ```

3. Generate the build files using CMake:
    ```sh
    cmake ..
    ```

4. Compile the project:
    ```sh
    make
    ```

### Using the Install Script

1. Navigate to the repository's root directory:
    ```sh
    cd <repository>
    ```

2. Run the install script:
    ```sh
    ./install.sh <path/to/media_folder>
    ```

## Usage

After building the project, start the media player with:
```sh
./ConsoleMediaPlayer
```

This will launch the media player in the terminal, where you can control playback, adjust volume, and speed up playback.

## Requirements

To ensure smooth operation of ConsoleMediaPlayer, install the following libraries:
```sh
sudo apt-get install g++ libncurses-dev libvlc-dev libx11-dev make cmake

sudo pacman -S gcc ncurses vlc libx11 make cmake

sudo dnf install gcc-c++ ncurses-devel vlc-devel libX11-devel make cmake

sudo emerge --sync
sudo emerge -av sys-devel/gcc sys-libs/ncurses media-libs/vlc x11-libs/libX11 sys-devel/make dev-util/cmake

sudo yum update
sudo yum install epel-release
sudo yum install gcc-c++ ncurses-devel vlc-devel libX11-devel make cmake
```

## Future Enhancements

ConsoleMediaPlayer is actively developed with more features planned for future releases. Stay tuned for updates!

## Contributing

Contributions are welcome! Feel free to fork the repository, make your changes, and submit a pull request.

