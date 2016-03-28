#include "glcontext.hpp"

template <typename T>
inline void setProc(const T &var, const char *name) {
    const_cast<T &>(var) = (T) SDL_GL_GetProcAddress(name);
}

glcontext::glcontext(SDL_Window *win) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    gl = SDL_GL_CreateContext(win);
    
    if (gl == nullptr) {
        const_cast<bool &>(isCore) = false;
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        gl = SDL_GL_CreateContext(win);
        if (gl == nullptr) logAndCrashSDL("SDL_GL_CreateContext");
    }
    
    setProc(GetUniformLocation, "glGetUniformLocation");
    setProc(GetAttribLocation, "glGetAttribLocation");
    setProc(GenVertexArrays, "glGenVertexArrays");
    setProc(DeleteVertexArrays, "glDeleteVertexArrays");
    setProc(GenBuffers, "glGenBuffers");
    setProc(DeleteBuffers, "glDeleteBuffers");
    setProc(BindVertexArray, "glBindVertexArray");
    setProc(UseProgram, "glUseProgram");
    setProc(BindBuffer, "glBindBuffer");
    setProc(BufferData, "glBufferData");
    setProc(EnableVertexAttribArray, "glEnableVertexAttribArray");
    setProc(VertexAttribPointer, "glVertexAttribPointer");
    setProc(ActiveTexture, "glActiveTexture");
    setProc(Uniform1ui,"glUniform1ui");
    setProc(Uniform1i, "glUniform1i");
    
    setProc(CreateShader, "glCreateShader");
    setProc(ShaderSource, "glShaderSource");
    setProc(CompileShader, "glCompileShader");
    setProc(DeleteShader, "glDeleteShader");
    setProc(GetShaderiv, "glGetShaderiv");
    setProc(GetShaderInfoLog, "glGetShaderInfoLog");
    setProc(CreateProgram, "glCreateProgram");
    setProc(AttachShader, "glAttachShader");
    setProc(LinkProgram, "glLinkProgram");
    setProc(GetProgramiv, "glGetProgramiv");
    setProc(GetProgramInfoLog, "glGetProgramInfoLog");
}

glcontext::~glcontext() {
    SDL_GL_DeleteContext(gl);
}

void glcontext::makeCurrent(SDL_Window *win) {
    SDL_GL_MakeCurrent(win, gl);
}

void glcontext::setTextureGrayscale(GLuint texture, SDL_Surface *surface) {
    ActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    checkGLError();
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    checkGLError();
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, surface->w, surface->h, 0, GL_RED, GL_UNSIGNED_BYTE, surface->pixels);
    checkGLError();
}

const std::string glcontext::getGLSLVersion() {
    if (isCore) return "#version 150\n";
    else return "#version 130\n";
}
