#pragma once

#ifndef __APPLICATION__
#define __APPLICATION__

#include "common/common.h"

class Application
{
public:
    Application(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera);
    virtual ~Application();

    virtual void Initialize();

    static std::unique_ptr<Application> CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera);
    static std::shared_ptr<class Scene> CreateScene();
    static std::shared_ptr<class Camera> CreateCamera();

    virtual std::unique_ptr<class Renderer> CreateRenderer();

    virtual glm::vec2 GetWindowSize() const;
    virtual bool IsFinished() const;
    virtual void RequestExit();

    virtual void Tick(double deltaTime);

    virtual void HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp);

    virtual void HandleWindowEvent(SDL_WindowEventID eventId, Sint32 data1, Sint32 data2, double timestamp);
protected:
    bool isRunning;

    std::shared_ptr<class Scene> scene;
    std::shared_ptr<class Camera> camera;

    virtual void HandleWindowResize(float x, float y);
private:
    // Setup scene will create the meshes, shaders and lights that will be needed to render the scene.
    virtual void SetupScene();
    virtual void SetupCamera();
};

#endif
