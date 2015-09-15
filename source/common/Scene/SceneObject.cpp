#include "common/Scene/SceneObject.h"
#include "common/Rendering/RenderingObject.h"

SceneObject::SceneObject(std::shared_ptr<class RenderingObject> baseObject):
    renderObject(std::move(baseObject))
{
}

SceneObject::~SceneObject()
{
}

void SceneObject::PrepareShaderForRendering() const
{
}

const RenderingObject& SceneObject::GetRenderObject() const
{
    return *renderObject.get();
}


