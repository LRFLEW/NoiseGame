#include "utilities.hpp"
#include "shaders.hpp"
#include "glcontext.hpp"
#include "polygons.hpp"
#include <random>
#include <bitset>

constexpr int SOLID_COUNT = 10;
constexpr int DELAY = 80;

constexpr int SOLID_WIDTH = 20;
constexpr int SOLID_HEIGHT = 20;

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

constexpr int FONT_SIZE = 42;
constexpr int BUFFER_HEIGHT = 80;
constexpr int LBUFFER = 20;

constexpr SDL_Rect WINDOW_SDLRECT = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT + BUFFER_HEIGHT };
constexpr SDL_Rect SCREEN_SDLRECT = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
constexpr Rect SCREEN_RECT = RectFromSDLRect(SCREEN_SDLRECT, WINDOW_SDLRECT);
constexpr RectTriangles SCREEN_RTRIS = RectToTriangles(SCREEN_RECT);

constexpr Rect UNIT_RECT = { 0.0f, 0.0f, 1.0f, 1.0f };
constexpr RectTriangles UV_RTRIS = RectToTriangles(UNIT_RECT);

inline void updateScoreTexture(glcontext &gl, TTF_Font *font, GLint tex, GLint pos, int cfound) {
    std::string str = std::to_string(cfound) + "/" + std::to_string(SOLID_COUNT);
    if (cfound >= SOLID_COUNT) str += "   You Win!  Replay?";
    SDL_Surface *scoresurf = TTF_RenderUTF8_Shaded(font, str.c_str(), { 255, 255, 255, 255 },
                                                   { 0, 0, 0, 255 });
    if (scoresurf == NULL) logAndCrashSDL("TTF_RenderText_Solid");
    
    gl.setTextureGrayscale(tex, scoresurf);
    
    SDL_Rect scoresize = { LBUFFER, SCREEN_HEIGHT + ((BUFFER_HEIGHT - scoresurf->h)/2), scoresurf->w, scoresurf->h };
    RectTriangles scorepos = RTrisFromSDLRect(scoresize, WINDOW_SDLRECT);
    SDL_FreeSurface(scoresurf);
    
    gl.BindBuffer(GL_ARRAY_BUFFER, pos);
    gl.BufferData(GL_ARRAY_BUFFER, sizeof(scorepos), &scorepos, GL_STATIC_DRAW);
    checkGLError();
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) logAndCrashSDL("SDL_Init");
    if (TTF_Init()) logAndCrashSDL("TTF_Init");
    
    SDL_Window *win = SDL_CreateWindow("Noise", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       WINDOW_SDLRECT.w, WINDOW_SDLRECT.h, SDL_WINDOW_OPENGL);
    if (win == NULL) logAndCrashSDL("SDL_CreateWindow");
    
    TTF_Font *font = TTF_OpenFont("OpenSans-Bold.ttf", FONT_SIZE);
    if (font == NULL) logAndCrashSDL("TFF_OpenFont");
    
    {
        glcontext gl(win);
        SDL_GL_SetSwapInterval(1);
        
        GLuint noiseprogram = loadProgram(gl, "noise");
        GLint noiseseedloc = gl.GetUniformLocation(noiseprogram, "seed");
        GLint noiseposloc = gl.GetAttribLocation(noiseprogram, "pos");
        checkGLError();
        
        GLuint texprogram = loadProgram(gl, "tex");
        GLint textexloc = gl.GetUniformLocation(texprogram, "tex");
        GLint texposloc = gl.GetAttribLocation(texprogram, "pos");
        GLint texuvloc = gl.GetAttribLocation(texprogram, "uv");
        
        GLuint vaos[3];
        gl.GenVertexArrays(3, vaos);
        checkGLError();
        
        GLuint pos[4];
        gl.GenBuffers(4, pos);
        checkGLError();
        
        GLuint tex;
        glGenTextures(1, &tex);
        checkGLError();
        
        gl.BindVertexArray(vaos[0]);
        gl.UseProgram(noiseprogram);
        gl.BindBuffer(GL_ARRAY_BUFFER, pos[0]);
        gl.BufferData(GL_ARRAY_BUFFER, sizeof(SCREEN_RTRIS), &SCREEN_RTRIS, GL_STATIC_DRAW);
        checkGLError();
        
        gl.EnableVertexAttribArray(noiseposloc);
        gl.VertexAttribPointer(noiseposloc, 2, GL_FLOAT, 0, 0, 0);
        checkGLError();
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        checkGLError();
        
        std::random_device rd;
        
        bool run = true;
        while (run) {
            std::mt19937 rand(rd());
            std::uniform_int_distribution<int> xpos(0, SCREEN_WIDTH - SOLID_WIDTH);
            std::uniform_int_distribution<int> ypos(0, SCREEN_HEIGHT - SOLID_HEIGHT);
            std::uniform_int_distribution<GLuint> seedgen;
            
            RectTriangles triangles[SOLID_COUNT];
            SDL_Rect solidloc[SOLID_COUNT];
            for (int i=0; i < SOLID_COUNT; ++i) {
                solidloc[i] = { xpos(rand), ypos(rand), SOLID_WIDTH, SOLID_HEIGHT};
                triangles[i] = RTrisFromSDLRect(solidloc[i], WINDOW_SDLRECT);
            }
            
            gl.BindVertexArray(vaos[1]);
            gl.UseProgram(noiseprogram);
            gl.BindBuffer(GL_ARRAY_BUFFER, pos[1]);
            gl.BufferData(GL_ARRAY_BUFFER, sizeof(triangles), &triangles, GL_STATIC_DRAW);
            checkGLError();
            
            gl.EnableVertexAttribArray(noiseposloc);
            gl.VertexAttribPointer(noiseposloc, 2, GL_FLOAT, 0, 0, 0);
            checkGLError();
            
            gl.BindVertexArray(vaos[2]);
            gl.UseProgram(texprogram);
            checkGLError();
            
            int cfound = 0;
            gl.ActiveTexture(GL_TEXTURE0);
            updateScoreTexture(gl, font, tex, pos[2], cfound);
            
            gl.EnableVertexAttribArray(texposloc);
            gl.VertexAttribPointer(texposloc, 2, GL_FLOAT, 0, 0, 0);
            checkGLError();
            
            gl.BindBuffer(GL_ARRAY_BUFFER, pos[3]);
            gl.BufferData(GL_ARRAY_BUFFER, sizeof(UV_RTRIS), &UV_RTRIS, GL_STATIC_DRAW);
            checkGLError();
            
            gl.EnableVertexAttribArray(texuvloc);
            gl.VertexAttribPointer(texuvloc, 2, GL_FLOAT, 0, 0, 0);
            checkGLError();
            
            bool runround = true;
            Uint32 delay = SDL_GetTicks() + DELAY;
            std::bitset<SOLID_COUNT> found;
            GLuint solidseed = seedgen(rand);
            GLuint seed = seedgen(rand);
            while (runround) {
                Uint32 ticks = SDL_GetTicks();
                if (delay <= ticks) {
                    delay += (ticks - delay) - ((ticks - delay) % DELAY) + DELAY;
                    seed = seedgen(rand);
                }
                
                bool dirtyscore = false;
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    switch (event.type) {
                        case SDL_MOUSEBUTTONDOWN:
                            if (event.button.y >= SCREEN_HEIGHT) {
                                runround = false;
                            } else for (int i=0; i < SOLID_COUNT; ++i) {
                                if (!found[i] && RectContainsPoint({ event.button.x, event.button.y }, solidloc[i])) {
                                    found[i] = true;
                                    ++cfound;
                                    dirtyscore = true;
                                    
                                    break;
                                }
                            }
                            break;
                        case SDL_QUIT:
                            runround = false;
                            run = false;
                            break;
                            
                        default:
                            break;
                    }
                }
                
                if (dirtyscore) updateScoreTexture(gl, font, tex, pos[2], cfound);
                
                glClear((GLbitfield) GL_COLOR_BUFFER_BIT);
                checkGLError();
                
                gl.BindVertexArray(vaos[0]);
                gl.UseProgram(noiseprogram);
                gl.Uniform1ui(noiseseedloc, seed);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                checkGLError();
                
                gl.BindVertexArray(vaos[1]);
                gl.UseProgram(noiseprogram);
                gl.Uniform1ui(noiseseedloc, solidseed);
                for (int i=0; i < SOLID_COUNT; ++i) {
                    if (!found[i]) glDrawArrays(GL_TRIANGLES, 6 * i, 6);
                }
                checkGLError();
                
                gl.BindVertexArray(vaos[2]);
                gl.UseProgram(texprogram);
                gl.Uniform1i(textexloc, 0);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                checkGLError();
                
                SDL_GL_SwapWindow(win);
            }
        }
        
        glDeleteTextures(1, &tex);
        gl.DeleteBuffers(4, pos);
        gl.DeleteVertexArrays(3, vaos);
    }
    
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyWindow(win);
    SDL_Quit();
    
    return 0;
}
