#include "window.hpp"

Window::Window(float aspectRatio, const std::string &title){ 
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    // bits for each pixel (at least)
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    // allocate space for second area to draw in
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // get display height
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    height = DM.h;
    width = float(height)*aspectRatio;

    sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                                            width, height, SDL_WINDOW_OPENGL);
    glContext = SDL_GL_CreateContext(sdlWindow);

    GLenum status = glewInit();
    if (status != GLEW_OK) fatalError("Glew failed to initialize");
    
    closed = false;
    
    // hide cursor
    SDL_ShowCursor(SDL_DISABLE);
    // keep cursor inside window
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

Window::~Window(){
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

void Window::update(){
    // Two buffers: one for drawing, one for displaying.
    // Swapped to show drawn image
    SDL_GL_SwapWindow(sdlWindow);
}

void Window::clear(float r, float g, float b, float a){
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}