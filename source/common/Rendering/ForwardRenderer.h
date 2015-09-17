#pragma once

#ifndef __FORWARD_RENDERER__
#define __FORWARD_RENDERER__

#include "common/Rendering/Renderer.h"

// This class provides a forward renderer WITHOUT lighting.
class ForwardRenderer: public Renderer
{
public:
    ForwardRenderer(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera);
    virtual ~ForwardRenderer();

    virtual void Initialize();
    virtual void Render();

protected:
    std::shared_ptr<class ShaderProgram> depthPrepassShader;
};

#endif
