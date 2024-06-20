# ConsoleMediaPlayer

ConsoleMediaPlayer is a CLI-based media player that offers basic playback controls, speed control, and volume adjustment. The functionality is planned to be extended over time, with the addition of more features.

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
- Enhanced user interface in the CLI
- More advanced media handling features

## Screenshots

### Preview
![Preview](screenshots/example.png)

### System Resource Usage
![System resource usage](screenshots/system_resource_usage.png)

## Building the Project

To build the ConsoleMediaPlayer from source, follow these steps:

### Prerequisites
Ensure that the following packages are installed on your system:
- GCC C++ compiler
- NCurses development libraries
- VLC development libraries
- X11 development libraries

Install the required packages on a Debian-based system using:
```sh
sudo apt-get install g++ libncurses-dev libvlc-dev libx11-dev
```

### Build Instructions

1. Navigate to the `src` directory:
    ```sh
    cd src/
    ```

2. Create a `build` directory:
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

Alternatively, you can use the `install.sh` script for installation:

### Using the Install Script

1. Navigate to the repository's root directory:
    ```sh
    cd <repository>
    ```

2. Run the install script with the desired path to the binary and the folder containing the media files:
    ```sh
    ./install.sh <path/to/media_folder>
    ```

## Usage

Once the project is built, you can start the media player using the following command:
```sh
./ConsoleMediaPlayer
```

This will launch the media player in the terminal, where you can use the provided controls to manage playback, adjust volume, and speed up the playback.

## Requirements

To ensure smooth operation of ConsoleMediaPlayer, the following libraries must be installed:
- GCC C++ compiler
- NCurses development libraries
- VLC development libraries
- X11 development libraries

Install these dependencies on a Debian-based system with:
```sh
sudo apt-get install g++ libncurses-dev libvlc-dev libx11-dev
```

## Future Enhancements

The ConsoleMediaPlayer is under active development, and more features are planned for future releases. Stay tuned for updates!
