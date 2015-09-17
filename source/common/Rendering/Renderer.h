#pragma once

#ifndef __RENDERER__
#define __RENDERER__

#include "common/common.h"

class Renderer
{
public:
    Renderer(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera);
    virtual ~Renderer();

    virtual void Initialize() = 0;
    virtual void Render() = 0;

protected:
    std::shared_ptr<class Scene> scene;
    std::shared_ptr<class Camera> camera;
};

#endif
