CC = g++
CFLAGS = -std=c++11 -Wall
LIBS = -lvlc -lX11 -lncurses
OBJS = main.o control.o CLI_menu.o

all: SPlayer

SPlayer: $(OBJS)
	$(CC) $(CFLAGS) -o SPlayer $(OBJS) $(LIBS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

control.o: control.cpp
	$(CC) $(CFLAGS) -c control.cpp

CLI_menu.o: CLI_menu.cpp
	$(CC) $(CFLAGS) -c CLI_menu.cpp

clean:
	rm -f $(OBJS) SPlayer
