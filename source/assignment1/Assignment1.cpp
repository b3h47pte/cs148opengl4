#include "assignment1/Assignment1.h"

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
