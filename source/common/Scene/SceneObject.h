#pragma once

#ifndef __SCENE_OBJECT__
#define __SCENE_OBJECT__

#include "common/common.h"

class SceneObject
{
public:
    SceneObject(std::shared_ptr<class RenderingObject> baseObject);
    virtual ~SceneObject();

    virtual void PrepareShaderForRendering() const;

    const class RenderingObject& GetRenderObject() const;
private:
    std::shared_ptr<class RenderingObject> renderObject;
};

#endif
