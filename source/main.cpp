#include "common/MediaLayer.h"
#include "common/Rendering/ForwardRenderer.h"
#include "FreeImage.h"

#if ASSIGNMENT == 1
#include "assignment1/Assignment1.h"
#define APPLICATION Assignment1
#elif ASSIGNMENT == 2
#include "assignment2/Assignment2.h"
#define APPLICATION Assignment2
#elif ASSIGNMENT == 3
#include "assignment3/Assignment3.h"
#define APPLICATION Assignment3
#else 
#define APPLICATION Application
#endif

#include <iostream>
#include <chrono>

int main(int argc, char** argv) {
    FreeImage_Initialise();

    std::unique_ptr<Application> app = APPLICATION::CreateApplication(APPLICATION::CreateScene(), APPLICATION::CreateCamera());
    if (!app) {
        std::cerr << "ERROR: Created application is not valid." << std::endl;
        return 1;
    }

    // The renderer depends on the camera and the scene generated by the application.
    // Make sure those are initialized first before continuing.
    std::unique_ptr<Renderer> renderer = app->CreateRenderer();
    if (!renderer) {
        std::cerr << "ERROR: Created renderer is not valid." << std::endl;
        return 1;
    }

    MediaLayer media(std::move(app), std::move(renderer));

    // NOTE: 'app' is no longer a valid pointer here. Make sure you do any initialization for your Application before
    // the MediaLayer is constructed.

    auto lastTickTime = std::chrono::high_resolution_clock::now();
    auto startTickTime = lastTickTime;
    while (media.CanTick()) {
        auto currentTickTime = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTickTime - lastTickTime);
        auto totalElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTickTime - startTickTime);
        media.Tick(deltaTime.count(), totalElapsedTime.count());
        lastTickTime = currentTickTime;
    }

    FreeImage_DeInitialise();
    return 0;
}
