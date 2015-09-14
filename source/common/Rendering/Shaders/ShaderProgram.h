#pragma once

#ifndef __SHADER_PROGRAM__
#define __SHADER_PROGRAM__

#include "common/common.h"

class ShaderProgram
{
public:
    ShaderProgram(const std::unordered_map<GLenum, std::string>& inputShaders);
    virtual ~ShaderProgram();

    void StartUseShader();
    void StopUseShader();

protected:
    static GLuint LoadShaderObject(GLenum type, const std::string& filename);

private:
    std::unordered_map<GLenum, GLuint> shaderObjects;
    GLuint shaderProgram;

    constexpr static int SHADER_ERROR_LOG_SIZE = 500;
};

#endif
