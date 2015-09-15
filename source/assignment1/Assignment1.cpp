#include "assignment1/Assignment1.h"
#include "common/core.h" // <-- haha.

Assignment1::Assignment1(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Application(std::move(inputScene), std::move(inputCamera))
{

}

std::unique_ptr<Application> Assignment1::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return std::make_unique<Assignment1>(std::move(scene), std::move(camera));
}

glm::vec2 Assignment1::GetWindowSize() const
{
    return glm::vec2(800.f, 800.f);
}

void Assignment1::SetupScene()
{
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "basicColor/basicColor.vert" },
        { GL_FRAGMENT_SHADER, "basicColor/basicColor.frag"}
    };
    std::shared_ptr<ShaderProgram> shader = std::make_shared<ShaderProgram>(shaderSpec);

    std::unique_ptr<RenderingObject::PositionArray> vertexPositions = std::make_unique<std::vector<glm::vec4>>(
        std::initializer_list<glm::vec4>({
            {0.f, 0.f, 0.f, 1.f},
            {1.f, 0.f, 0.f, 1.f},
            {1.f, 1.f, 0.f, 1.f}
        })
    );

    std::shared_ptr<RenderingObject> triangleTemplate = std::make_shared<RenderingObject>(shader, std::move(vertexPositions));
    std::shared_ptr<SceneObject> sceneTriangle = std::make_shared<SceneObject>(triangleTemplate);
    scene->AddSceneObject(std::move(sceneTriangle));
}
