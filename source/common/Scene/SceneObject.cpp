#include "common/Scene/SceneObject.h"
#include "common/Scene/Camera/Camera.h"
#include "common/Rendering/RenderingObject.h"

const std::string SceneObject::MODEL_MATRIX_LOCATION = "modelMatrix";
const std::string SceneObject::VIEW_MATRIX_LOCATION = "viewMatrix";
const std::string SceneObject::PROJECTION_MATRIX_LOCATION = "projectionMatrix";

#define VERIFY_SHADER_UNIFORM_EXISTS(program, name, var) \
    GLint var = glGetUniformLocation(program, name); \
    if (!var) { \
        return; \
    } \

SceneObject::SceneObject():
    cachedTransformationMatrix(1.f), rotation(1.f, 0.f, 0.f, 0.f), scale(1.f)
{
}

SceneObject::SceneObject(std::shared_ptr<class RenderingObject> baseObject):
    renderObject(std::move(baseObject))
{
}

SceneObject::~SceneObject()
{
}

void SceneObject::PrepareShaderForRendering(GLuint program, const Camera* currentCamera) const
{
    // Send the model, view, and projection matrix to the shader only if the shader
    // requests those variables.
    SetShaderUniform(program, MODEL_MATRIX_LOCATION, GetTransformationMatrix());
    SetShaderUniform(program, VIEW_MATRIX_LOCATION, currentCamera->GetTransformationMatrix());
    SetShaderUniform(program, PROJECTION_MATRIX_LOCATION, currentCamera->GetProjectionMatrix());
}

const RenderingObject* SceneObject::GetRenderObject() const
{
    return renderObject.get();
}

glm::mat4 SceneObject::GetTransformationMatrix() const
{
    return cachedTransformationMatrix;
}

void SceneObject::UpdateTransformationMatrix()
{
    glm::mat4 newTransformation(1.f);
    newTransformation = glm::scale(newTransformation, scale);
    newTransformation = glm::mat4_cast(rotation) * newTransformation;
    newTransformation = glm::translate(newTransformation, glm::vec3(position) / position.w);
    cachedTransformationMatrix = std::move(newTransformation);
}

glm::vec4 SceneObject::GetForwardDirection() const
{
    return glm::mat4_cast(rotation) * GetWorldForward();
}

glm::vec4 SceneObject::GetWorldUp()
{
    return glm::vec4(0.f, 1.f, 0.f, 0.f);
}

glm::vec4 SceneObject::GetWorldRight()
{
    return glm::vec4(1.f, 0.f, 0.f, 0.f);
}

glm::vec4 SceneObject::GetWorldForward()
{
    return glm::vec4(0.f, 0.f, -1.f, 0.f);
}

void SceneObject::SetShaderUniform(GLuint program, const std::string& location, const glm::mat4& value) const
{
    VERIFY_SHADER_UNIFORM_EXISTS(program, location.c_str(), uniformLocation);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
}
