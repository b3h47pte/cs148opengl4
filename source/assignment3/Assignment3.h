#pragma once

#ifndef __ASSIGNMENT_3__
#define __ASSIGNMENT_3__

#include "common/Application.h"

class Assignment3: public Application
{
public:
    Assignment3(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera);

    static std::unique_ptr<Application> CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera);
    static std::shared_ptr<class Camera> CreateCamera();

    virtual glm::vec2 GetWindowSize() const;

    virtual void HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime);
    virtual void Tick(double deltaTime);
protected:
    virtual void HandleWindowResize(float x, float y);

private:
    virtual void SetupScene();

    virtual void SetupExample1();
    virtual void SetupCamera();

    std::shared_ptr<class SceneObject> sceneObject;

    std::shared_ptr<class Light> pointLight;
    std::shared_ptr<class Light> pointLight2;
    std::shared_ptr<class Light> pointLight3;
    std::shared_ptr<class Light> pointLight4;
    float elapsedTime;
};

#endif
