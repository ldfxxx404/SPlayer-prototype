**What can this player do?**

1. Control playback (pause, play, mute).
2. Speed up playback (X2)
3. Volume control

P.S. Functionality will be extended, and the player itself will get a ~~graphical~~ CLI interface.

---

**Screenshots**

![Preview](screenshots/example.png)

![System resource usage](screenshots/system_resource_usage.png)

## building 

```sh
cd src/
mkdir build
cd build
cmake ..
make
```
# OR u can use install.sh script

```sh
cd <repos>
./install.sh <path/to/bin> (folder with media)
```
## usage

```sh
./ConsoleMediaPlayer
```
## 
```sh
sudo apt-get install gcc-c++ ncurses-devel vlc-devel xorg-x11-devel
```
