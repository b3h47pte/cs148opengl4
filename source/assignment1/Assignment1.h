#pragma once

#ifndef __ASSIGNMENT_1__
#define __ASSIGNMENT_1__

#include "common/Application.h"

class Assignment1: public Application
{
public:
    Assignment1(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera);

    static std::unique_ptr<Application> CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera);

    virtual glm::vec2 GetWindowSize() const;
private:
    virtual void SetupScene();
};

#endif
