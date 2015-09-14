#pragma once

#ifndef __RENDERING_OBJECT__
#define __RENDERING_OBJECT__

#include "common/common.h"

// This class holds the data for each vertex and all of its attributes and its equivalent OpenGL representation.
class RenderingObject
{
public:
    enum class VertexAttributePositions {
        Position = 0,
        Normals,
        UV,
        Colors
    };

    RenderingObject(std::shared_ptr<class ShaderProgram> inputShader,
        std::unique_ptr<std::vector<glm::vec4>> inputPositions,
        std::unique_ptr<std::vector<int>> inputIndices = nullptr,
        std::unique_ptr<std::vector<glm::vec3>> inputNormals = nullptr,
        std::unique_ptr<std::vector<glm::vec2>> inputUV = nullptr,
        std::unique_ptr<std::vector<glm::vec4>> inputColors = nullptr);
    ~RenderingObject();

    RenderingObject(const RenderingObject&) = delete;
    RenderingObject& operator=(const RenderingObject&) = delete;

private:
    std::shared_ptr<class ShaderProgram> shader;

    GLuint vertexPositionBuffer;
    std::unique_ptr<std::vector<glm::vec4>> vertexPositions;

    GLuint vertexIndexBuffer;
    std::unique_ptr<std::vector<int>> vertexIndices;

    GLuint vertexNormalBuffer;
    std::unique_ptr<std::vector<glm::vec3>> vertexNormals;

    GLuint vertexUVBuffer;
    std::unique_ptr<std::vector<glm::vec2>> vertexUV;

    GLuint vertexColorBuffer;
    std::unique_ptr<std::vector<glm::vec4>> vertexColors;

    void InitializeOpenGL();

    GLuint vao;
};

#endif
