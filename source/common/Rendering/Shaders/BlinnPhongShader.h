#pragma once

#ifndef __BLINN_PHONG_SHADER__
#define __BLINN_PHONG_SHADER__

#include "common/Rendering/Shaders/ShaderProgram.h"
#include <functional>

// CPU Interface to the BlinnPhong shader (either or: vert or frag version).
class BlinnPhongShader: public ShaderProgram
{
public:
    BlinnPhongShader(const std::unordered_map<GLenum, std::string>& inputShaders, GLenum lightingStage);
    virtual ~BlinnPhongShader();

    virtual void SetupShaderLighting(const class Light* light) const; 
    virtual void SetupShaderMaterials() const;
    virtual void SetupShaderCamera(const class Camera* camera) const;

    virtual void SetDiffuse(glm::vec4 inDiffuse);
    virtual void SetSpecular(glm::vec4 inSpecular, float inShininess);
    virtual void SetAmbient(glm::vec4 inAmbient);

    struct TextureSlots {
        enum Type {
            DIFFUSE = 0,
            SPECULAR
        };
    };
    virtual void SetTexture(TextureSlots::Type slot, std::shared_ptr<class Texture> inputTexture);

    static std::unique_ptr<struct BlinnPhongLightProperties> CreateLightProperties();
protected:
    // Material Parameters
    virtual void UpdateMaterialBlock() const;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float shininess;
    glm::vec4 ambient;

    // Material Bindings into the Shader
    static std::array<const char*, 4> MATERIAL_PROPERTY_NAMES;
    static constexpr int MATERIAL_BINDING_POINT = 0;
    GLuint materialBlockLocation;
    GLint materialBlockSize;
    std::array<GLuint, 4> materialIndices;
    std::array<GLint, 4> materialOffsets;
    GLuint materialBuffer;
    std::vector<GLubyte> materialStorage;

    // Attenuation Uniform Handling
    virtual void UpdateAttenuationUniforms(const class Light* light) const;

private:
    // Textures
    std::shared_ptr<class Texture> defaultTexture;
    std::unordered_map<TextureSlots::Type, std::shared_ptr<class Texture>, std::hash<int> > textureSlotMapping;

    GLenum lightingShaderStage;
};


#endif
