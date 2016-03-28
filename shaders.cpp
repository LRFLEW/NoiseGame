#include "shaders.hpp"

GLuint loadProgram(glcontext &gl, std::string prefix) {
    std::string version = gl.getGLSLVersion();
    size_t vsize = version.size();
    
    SDL_RWops *vertRW = SDL_RWFromFile((prefix + "Vert.glsl").c_str(), "r");
    GLint vertSize = (GLint) SDL_RWsize(vertRW);
    if (vertSize < 0) logAndCrashSDL("SDL_RWsize");
    char *vertexShader = new char[vertSize + vsize];
    std::strcpy(vertexShader, version.c_str());
    SDL_RWread(vertRW, vertexShader + vsize, 1, vertSize);
    SDL_RWclose(vertRW);
    vertSize += vsize;
    
    SDL_RWops *fragRW = SDL_RWFromFile((prefix + "Frag.glsl").c_str(), "r");
    GLint fragSize = (GLint) SDL_RWsize(fragRW);
    if (fragSize < 0) logAndCrashSDL("SDL_RWsize");
    char *fragmentShader = new char[fragSize + vsize];
    std::strcpy(fragmentShader, version.c_str());
    SDL_RWread(fragRW, fragmentShader + vsize, 1, fragSize);
    SDL_RWclose(fragRW);
    fragSize += vsize;
    
    GLuint vertShader = gl.CreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = gl.CreateShader(GL_FRAGMENT_SHADER);
    checkGLError();
    
    GLint result = GL_FALSE;
    
    gl.ShaderSource(vertShader, 1, (const GLchar **) &vertexShader, &vertSize);
    gl.CompileShader(vertShader);
    
    gl.GetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        GLint logLength;
        gl.GetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
        GLchar *error = new char[logLength];
        gl.GetShaderInfoLog(vertShader, logLength, NULL, error);
        SDL_SetError("%s", error);
        logAndCrashSDL("Compiling Vertex Shader");
        delete [] error;
    }
    
    gl.ShaderSource(fragShader, 1, (const GLchar **) &fragmentShader, &fragSize);
    gl.CompileShader(fragShader);
    
    gl.GetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        GLint logLength;
        gl.GetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
        GLchar *error = new GLchar[logLength];
        gl.GetShaderInfoLog(fragShader, logLength, NULL, error);
        SDL_SetError("%s", error);
        logAndCrashSDL("Compiling Fragment Shader");
        delete [] error;
    }
    checkGLError();
    
    GLuint program = gl.CreateProgram();
    gl.AttachShader(program, vertShader);
    gl.AttachShader(program, fragShader);
    gl.LinkProgram(program);
    
    gl.GetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result) {
        GLint logLength;
        gl.GetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        GLchar *error = new GLchar[logLength];
        gl.GetProgramInfoLog(program, logLength, NULL, error);
        SDL_SetError("%s", error);
        logAndCrashSDL("Linking Shaders");
        delete [] error;
    }
    checkGLError();
    
    gl.DeleteShader(vertShader);
    gl.DeleteShader(fragShader);
    checkGLError();
    
    delete [] vertexShader;
    delete [] fragmentShader;
    
    return program;
}
