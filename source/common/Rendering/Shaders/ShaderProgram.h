#pragma once

#ifndef __SHADER_PROGRAM__
#define __SHADER_PROGRAM__

#include "common/common.h"

class ShaderProgram
{
public:
    ShaderProgram(const std::unordered_map<GLenum, std::string>& inputShaders);
    virtual ~ShaderProgram();

    virtual void StartUseShader() const;
    virtual void StopUseShader() const;

    GLuint GetProgram() const { return shaderProgram; }

    virtual void SetupShaderLighting(const class Light* light) const; 
    virtual void SetupShaderMaterials() const;
    virtual void SetupShaderCamera(const class Camera* camera) const;

    // Shader Interface
    void SetShaderUniform(const std::string& location, const glm::mat4& value) const;
    void SetShaderUniform(const std::string& location, float value) const;
    void SetShaderUniform(const std::string& location, const glm::vec4& value) const;
    void SetShaderSubroutine(const std::string& location, const std::string& subroutine, GLenum substage) const;

    template<int N>
    void SetupUniformBlock(const std::string& blockName,
        const std::array<const char*, N>& names, 
        std::array<GLuint, N>& indices,
        std::array<GLint, N>& offsets,
        std::vector<GLubyte>& data,
        GLuint& blockLocation, GLint& blockSize, GLuint& bufferLocation)
    {
        StartUseShader();

        blockLocation = OGL_CALL(glGetUniformBlockIndex(shaderProgram, blockName.c_str()));
        if (blockLocation == GL_INVALID_INDEX) {
            return;
        }

        OGL_CALL(glGetActiveUniformBlockiv(shaderProgram, blockLocation, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize));
        data.resize(blockSize);

        OGL_CALL(glGetUniformIndices(shaderProgram, N, names.data(), indices.data()));
        OGL_CALL(glGetActiveUniformsiv(shaderProgram, N, indices.data(), GL_UNIFORM_OFFSET, offsets.data()));

        OGL_CALL(glGenBuffers(1, &bufferLocation));

        StopUseShader();  
    }

    static std::unique_ptr<struct LightProperties> CreateLightProperties();
protected:
    static GLuint LoadShaderObject(GLenum type, const std::string& filename);

    GLuint shaderProgram;
private:
    std::unordered_map<GLenum, GLuint> shaderObjects;

    constexpr static int SHADER_ERROR_LOG_SIZE = 500;
};

#endif
