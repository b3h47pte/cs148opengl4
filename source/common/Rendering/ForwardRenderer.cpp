#include "common/Rendering/ForwardRenderer.h"
#include "common/Scene/Scene.h"
#include "common/Scene/SceneObject.h"
#include "common/Scene/Camera/Camera.h"
#include "common/Rendering/RenderingObject.h"

#include <algorithm>

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
    auto totalLights = scene->GetTotalLights() + 1;

    for (decltype(totalLights) l = 0; l < totalLights; ++l) {
        OGL_CALL(glClear(GL_DEPTH_BUFFER_BIT));

        // Start from the end because the very 'last' light handles the global lighting for the object.
        // A light that is a nullptr will cause the shader to perform default behavior, whatever that may be.
        const Light* lightObject = scene->GetLightObject(totalLights - l - 1);

        for (decltype(totalObjects) i = 0; i < totalObjects; ++i) {
            const SceneObject& sceneObject = scene->GetSceneObject(i); 
            const RenderingObject* renderObject = sceneObject.GetRenderObject();
            if (!renderObject) {
                continue;
            }

            renderObject->BeginRender();

            // Make the scene object setup its shader with any data its shader might need.
            sceneObject.PrepareShaderForRendering(renderObject->GetShaderProgramRaw(), camera.get(), lightObject);
            renderObject->Render();

            renderObject->EndRender();
        }
    }
}
