#pragma once

#ifndef __SCENE__
#define __SCENE__

#include "common/common.h"

class SceneObject;

class Scene: public std::enable_shared_from_this<Scene>
{
public:
    decltype(auto) GetTotalObjects() const 
    { 
        return sceneObjects.size(); 
    }

    template<typename T, std::enable_if_t<std::is_integral<T>::value>* = nullptr>
    const SceneObject* GetRawSceneObject(T index) const
    {
        const std::shared_ptr<SceneObject>& internalObject = sceneObjects[index];
        return internalObject.get();
    }

    friend class Renderer;
private:
    std::vector<std::shared_ptr<SceneObject>> sceneObjects;
};

#endif
