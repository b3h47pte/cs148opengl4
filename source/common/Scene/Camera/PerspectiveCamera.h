#pragma once

#ifndef __PERSPECTIVE_CAMERA__
#define __PERSPECTIVE_CAMERA__

#include "common/Scene/Camera/Camera.h"

class PerspectiveCamera: public Camera
{
public:
    PerspectiveCamera(float inFov, float inAR);

    virtual glm::mat4 GetProjectionMatrix() const;

    virtual float GetFOV() const { return fov; }
    virtual void SetFOV(float in) {fov = in;}

    virtual float GetAspectRatio() const { return aspectRatio; }
    virtual void SetAspectRatio(float in) { aspectRatio = in; }

    virtual float GetZNear() const { return zNear; }
    virtual void SetZNear(float in) { zNear = in; }

    virtual float GetZFar() const { return zFar; }
    virtual void SetZFar(float in) { zFar = in; }
protected:
    virtual void UpdateTransformationMatrix();

private:
    float fov;
    float aspectRatio;
    float zNear;
    float zFar;
};

#endif
