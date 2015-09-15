#include "common/Scene/Scene.h"

void Scene::AddSceneObject(std::shared_ptr<SceneObject> object)
{
    sceneObjects.emplace_back(std::move(object));
}
