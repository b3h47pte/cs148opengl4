#include "common/Rendering/ForwardRenderer.h"
#include "common/Scene/Scene.h"
#include "common/Scene/SceneObject.h"
#include "common/Scene/Camera/Camera.h"
#include "common/Rendering/RenderingObject.h"

ForwardRenderer::ForwardRenderer(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Renderer(std::move(inputScene), std::move(inputCamera))
{
}

ForwardRenderer::~ForwardRenderer()
{
}

void ForwardRenderer::Render()
{
    auto totalObjects = scene->GetTotalObjects();
    for (decltype(totalObjects) i = 0; i < totalObjects; ++i) {
        const SceneObject& sceneObject = scene->GetSceneObject(i); 
        const RenderingObject* renderObject = sceneObject.GetRenderObject();
        if (!renderObject) {
            continue;
        }

        renderObject->BeginRender();

        // Make the scene object setup its shader with any data its shader might need.
        sceneObject.PrepareShaderForRendering(renderObject->GetShaderProgram(), camera.get());
        renderObject->Render();

        renderObject->EndRender();
    }
}
