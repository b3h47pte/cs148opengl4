#pragma once

#ifndef __SCENE_OBJECT__
#define __SCENE_OBJECT__

#include "common/common.h"

class SceneObject: public std::enable_shared_from_this<SceneObject>
{
public:
    SceneObject();
    SceneObject(std::shared_ptr<class RenderingObject> baseObject);
    SceneObject(const std::vector<std::shared_ptr<class RenderingObject>>& baseObjects);
    virtual ~SceneObject();

    virtual void PrepareShaderForRendering(const class ShaderProgram* shader, const class Camera* currentCamera, const class Light* currentLight) const;

    size_t GetTotalRenderObjects() const { return renderObject.size(); }
    virtual const class RenderingObject* GetRenderObject(int index = 0) const;

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

    void SetPosition(const glm::vec3& in);

    //
    // Individual transform retrieval.
    //
    glm::vec4 GetPosition() const { return position; }
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

private:
    std::vector<std::shared_ptr<class RenderingObject>> renderObject;
};

#endif
