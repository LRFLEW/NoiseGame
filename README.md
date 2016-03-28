# Noise Game
This short game demo was originally created during a 4-hour game jam. I later rewrote the graphics code to utilize OpenGL for rendering instead of the original CPU-driven technique. The idea of the game is to see if a game can be made using white noise as the primary graphic.

When you launch the game, You will be presented with a screen filled with constantly-changing white noise. In the white noise, there are 10 squares that are not changing. The goal of the game is to find and click on all 10 static squares. Below the static is a score counter. You can click on the counter at any time to reset the game.

## Building

This game requires SDL2 and SDL2\_ttf to build. Additionally, the included build script requires CMake and pkg-config. The program requires OpenGL 3.0 or higher to run.

To build the game on nix systems, run `cmake . && make` and run the game with `./Noise`. The instructions for building on Windows will depend on the build system you plan on using (Visual Studio or MinGW).
