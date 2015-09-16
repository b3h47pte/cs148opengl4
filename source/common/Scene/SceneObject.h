#pragma once

#ifndef __SCENE_OBJECT__
#define __SCENE_OBJECT__

#include "common/common.h"

class SceneObject: public std::enable_shared_from_this<SceneObject>
{
public:
    SceneObject();
    SceneObject(std::shared_ptr<class RenderingObject> baseObject);
    virtual ~SceneObject();

    virtual void PrepareShaderForRendering(GLuint program, const class Camera* currentCamera) const;

    virtual const class RenderingObject* GetRenderObject() const;

    virtual glm::mat4 GetTransformationMatrix() const;

    virtual glm::vec4 GetForwardDirection() const;
    virtual glm::vec4 GetRightDirection() const;
    virtual glm::vec4 GetUpDirection() const;

    static glm::vec4 GetWorldUp();
    static glm::vec4 GetWorldRight();
    static glm::vec4 GetWorldForward();

    //
    // Functions to transform the object.
    //
    void Translate(const glm::vec3& translation);
    void Rotate(const glm::vec3& axis, float radians);
    void MultScale(float inputScale);
    void AddScale(float inputScale);
protected:
    virtual void UpdateTransformationMatrix();
    glm::mat4 cachedTransformationMatrix;

    glm::vec4 position;
    glm::quat rotation;
    glm::vec3 scale;

    static const std::string MODEL_MATRIX_LOCATION;
    static const std::string VIEW_MATRIX_LOCATION;
    static const std::string PROJECTION_MATRIX_LOCATION;
    static constexpr float MINIMUM_SCALE = 0.01f;

    // Shader Interface
    void SetShaderUniform(GLuint program, const std::string& location, const glm::mat4& value) const;
private:
    std::shared_ptr<class RenderingObject> renderObject;
};

#endif
