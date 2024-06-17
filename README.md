**What can this player do?**

1. Control playback (pause, play, mute).
2. Speed up playback (X2)
3. Volume control

P.S. Functionality will be extended, and the player itself will get a ~~graphical~~ CLI interface.

---

**Screenshots**

![Preview](screenshots/example.png)

![System resource usage](screenshots/system_resource_usage.png)

##How to use

```bash
$ git clone <repos>
$ cd repos
$ mkdir bin 
$ g++ main.cpp -o ../bin/SimplePlayer -lvlc -lX11 -lncurses -ltinfo
$ cd bin 
$ ./SimplePlayer
```