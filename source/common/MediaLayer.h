//
// Class that abstracts the boilerplate to initialize SDL and 
// setup the OpenGL context.
//
#pragma once

#ifndef __MEDIA_LAYER__
#define __MEDIA_LAYER__

#include "common/common.h"
#include "common/Application.h"
#include "common/Rendering/Renderer.h"
#include <memory>

class MediaLayer
{
public:
    MediaLayer(std::unique_ptr<Application> inputApp, std::unique_ptr<Renderer> inputRenderer);
    ~MediaLayer();

    bool CanTick() const;
    void Tick(double deltaTime, double currentTime);

private:
    // Underlying Application to run
    std::unique_ptr<Application> app;

    // Underlying Renderer to run
    std::unique_ptr<Renderer> renderer;

    // SDL 
    void InitializeSDL();
    SDL_Window* sdlWindow;
    bool sdlInitialized;

    // OpenGL
    void InitializeOpenGL();
    SDL_GLContext glContext;
    bool openglInitialized;
};

#endif
