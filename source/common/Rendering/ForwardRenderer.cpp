#include "common/Rendering/ForwardRenderer.h"

ForwardRenderer::ForwardRenderer(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Renderer(inputScene, inputCamera)
{
}

ForwardRenderer::~ForwardRenderer()
{
}
