#ifndef __Noise__utilities__
#define __Noise__utilities__

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_ttf.h>

void SDL_Log(SDL_LogPriority priority, const char* fmt, ...);
void logAndCrashSDL(const char* error);
void checkGLError();

#endif /* defined(__Noise__utilities__) */
