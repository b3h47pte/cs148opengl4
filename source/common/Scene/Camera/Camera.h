#pragma once

#ifndef __CAMERA__
#define __CAMERA__

#include "common/Scene/SceneObject.h"

class Camera: public SceneObject
{
public:
    Camera();
    
    virtual glm::mat4 GetProjectionMatrix() const;
protected:
    virtual void UpdateTransformationMatrix();
};


#endif
