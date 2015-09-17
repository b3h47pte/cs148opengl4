#pragma once

#ifndef __LIGHT__
#define __LIGHT__

#include "common/Scene/SceneObject.h"

// By default the light acts like a point light.
class Light: public SceneObject
{
public:
    enum class LightType {
        POINT 
    };

    Light(std::unique_ptr<struct LightProperties> inProperties, LightType type = LightType::POINT);
    virtual ~Light();

    void GetAttenuation(float& constant, float& linear, float& quadratic) const;

    LightType GetLightType() const { return lightType; }

    const struct LightProperties* GetPropertiesRaw() const;
    
    virtual void SetupShaderUniforms(const class ShaderProgram* program) const;
private:
    static const std::string LIGHT_UNIFORM_NAME;
    std::unique_ptr<struct LightProperties> properties;

    LightType lightType;

    // Attenuation
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

#endif
