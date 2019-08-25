# gamedev2

gamedev2 is a reincarnation of a previous 
    [project](github.com/tcsullivan/gamedev) created by 
    [Andy Belle-Isle](github.com/drumsetmonkey) and 
    [Clyne Sullivan](github.com/tcsullivan).

After a long time developing [gamedev](github.com/tcsullivan/gamedev),
we decided a large amount of the codebase was rather messy and inefficient.
Instead of rewriting gamedev piece by piece, we decided that the final
product would be of a much higher quality if the project was restarted from
scratch.

## Goal
The goal of gamedev2 is to create a commercial-grade video game in which content
can be created through the use of scripting languages such as Lua. One of the
things that sets gamedev2 apart from many other indie games is that gamedev2
is an engine in itself. By creating the engine behind the game, we are able to
implement only the necessary features needed to make gamedev2 run smoothly
without the extra bloat brought on by a general purpose game engine.

## Features (Planned)
* Fully modifiable through the use of scripting languages
* Multiplayer

## Libraries
The libraries used to develop gamedev2 are as follows:
* SDL2
* OpenGL
* GLEW
* GLU
* Lua
* EntityX
* FreeType 2


## Building gamedev2
### Build Requirements
gamedev2 is actively developed on Linux, so the Windows build process may not
be as simple as the Linux requirements listed below.
In order to build gamedev2 the following programs are needed:
* Make
* g++ (version >7)
* 64-Bit OS support

### Linux
In order to build gamedev2 on Linux, the process is fairly simple.
1. First cd into the root gamedev2 directory
```
cd gamedev2
```

2. EntityX must be build first before the rest of gamedev2
```
cd lib/entityx
make
cd ../..
```

3. To build gamedev2 itself the following can be run:
```
make
```
In order to make gamedev2 using multiple threads, use the following instead:
```
make -jN
```
with 'N' being the number of threads to use while building.

4. Once built, gamedev2 can be run with
```
./main
```
