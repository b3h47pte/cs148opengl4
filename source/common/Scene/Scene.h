#pragma once

#ifndef __SCENE__
#define __SCENE__

#include "common/common.h"

class SceneObject;
class Light;

class Scene: public std::enable_shared_from_this<Scene>
{
public:
    decltype(auto) GetTotalObjects() const 
    { 
        return sceneObjects.size(); 
    }

    decltype(auto) GetTotalLights() const
    {
        return sceneLights.size();
    }

    template<typename T, std::enable_if_t<std::is_integral<T>::value>* = nullptr>
    const SceneObject& GetSceneObject(T index) const
    {
        assert(index >= 0 && index < sceneObjects.size());
        const std::shared_ptr<SceneObject>& internalObject = sceneObjects[index];
        return *internalObject.get();
    }

    template<typename T, std::enable_if_t<std::is_integral<T>::value>* = nullptr>
    const Light* GetLightObject(T index) const
    {
        if (index >= 0 && index < sceneLights.size()) {
            return sceneLights[index].get();
        }
        return nullptr;
    }

    void AddSceneObject(std::shared_ptr<SceneObject> object);
    void AddLight(std::shared_ptr<Light> light);

    void ClearScene();

    friend class Renderer;
private:
    std::vector<std::shared_ptr<SceneObject>> sceneObjects;
    std::vector<std::shared_ptr<Light>> sceneLights;
};

#endif
