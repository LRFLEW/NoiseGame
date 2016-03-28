#include "utilities.hpp"
#include <cstdlib>

void SDL_Log(SDL_LogPriority priority, const char* fmt, ...) {
    va_list ap;
    
    va_start(ap, fmt);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, priority, fmt, ap);
    va_end(ap);
}

void logAndCrashSDL(const char* error) {
    const char* msg = SDL_GetError();
    SDL_Log(SDL_LOG_PRIORITY_CRITICAL, "%s: %s", error, msg);
    if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, error, msg, NULL))
        SDL_Log(SDL_LOG_PRIORITY_CRITICAL, "Error presenting error");
    SDL_Quit();
    std::exit(-1);
}

void checkGLError() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        SDL_SetError("GL Error 0x%04X", error);
        logAndCrashSDL("OpenGL Error");
    }
}
