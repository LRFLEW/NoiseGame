#ifndef __Noise__glcontext__
#define __Noise__glcontext__

#include "utilities.hpp"
#include <string>

class glcontext {
public:
    glcontext(SDL_Window *win);
    ~glcontext();
    void makeCurrent(SDL_Window *win);
    
    glcontext(glcontext const &gl) = delete;
    void operator=(glcontext const &gl) = delete;
    
    void setTextureGrayscale(GLuint texture, SDL_Surface *surface);
    
    const std::string getGLSLVersion();
    
    const PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation = nullptr;
    const PFNGLGETATTRIBLOCATIONPROC GetAttribLocation = nullptr;
    const PFNGLGENVERTEXARRAYSPROC GenVertexArrays = nullptr;
    const PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays = nullptr;
    const PFNGLGENBUFFERSPROC GenBuffers = nullptr;
    const PFNGLDELETEBUFFERSPROC DeleteBuffers = nullptr;
    const PFNGLBINDVERTEXARRAYPROC BindVertexArray = nullptr;
    const PFNGLUSEPROGRAMPROC UseProgram = nullptr;
    const PFNGLBUFFERDATAPROC BufferData = nullptr;
    const PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray = nullptr;
    const PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer = nullptr;
    const PFNGLACTIVETEXTUREPROC ActiveTexture = nullptr;
    const PFNGLUNIFORM1UIPROC Uniform1ui = nullptr;
    const PFNGLUNIFORM1IPROC Uniform1i = nullptr;
    
    const PFNGLCREATESHADERPROC CreateShader = nullptr;
    const PFNGLSHADERSOURCEPROC ShaderSource = nullptr;
    const PFNGLCOMPILESHADERPROC CompileShader = nullptr;
    const PFNGLDELETESHADERPROC DeleteShader = nullptr;
    const PFNGLGETSHADERIVPROC GetShaderiv = nullptr;
    const PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog = nullptr;
    const PFNGLCREATEPROGRAMPROC CreateProgram = nullptr;
    const PFNGLATTACHSHADERPROC AttachShader = nullptr;
    const PFNGLLINKPROGRAMPROC LinkProgram = nullptr;
    const PFNGLGETPROGRAMIVPROC GetProgramiv = nullptr;
    const PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog = nullptr;
    const PFNGLBINDBUFFERPROC BindBuffer = nullptr;
private:
    SDL_GLContext gl;
    const bool isCore = true;
};

#endif /* defined(__Noise__glcontext__) */
