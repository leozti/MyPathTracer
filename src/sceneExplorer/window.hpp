#pragma once

#include "mySceneExp.hpp"

class Window {
    public:
        Window(float aspectRatio, const std::string &title);
        
        ~Window();
        
        void update();

        bool isClosed(){return closed;}

        void clear(float r, float g, float b, float a);

        void close(){closed = true;}

        int getWidth(){return width;}
        int getHeigth(){return height;}

    private:
        SDL_Window *sdlWindow;
        SDL_GLContext glContext;
        int width;
        int height;
        bool closed;
};