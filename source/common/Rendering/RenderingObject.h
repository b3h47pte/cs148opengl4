#pragma once

#ifndef __RENDERING_OBJECT__
#define __RENDERING_OBJECT__

#include "common/common.h"

// This class holds the data for each vertex and all of its attributes and its equivalent OpenGL representation.
class RenderingObject
{
public:
    using PositionArray = std::vector<glm::vec4>;
    using IndexArray = std::vector<unsigned int>;
    using NormalArray = std::vector<glm::vec3>;
    using UVArray = std::vector<glm::vec2>;
    using ColorArray = std::vector<glm::vec4>;

    enum class VertexAttributePositions {
        Position = 0,
        Normals,
        UV,
        Colors
    };

    RenderingObject(std::shared_ptr<class ShaderProgram> inputShader,
        std::unique_ptr<PositionArray> inputPositions,
        std::unique_ptr<IndexArray> inputIndices = nullptr,
        std::unique_ptr<NormalArray> inputNormals = nullptr,
        std::unique_ptr<UVArray> inputUV = nullptr,
        std::unique_ptr<ColorArray> inputColors = nullptr);
    virtual ~RenderingObject();

    RenderingObject(const RenderingObject&) = delete;
    RenderingObject& operator=(const RenderingObject&) = delete;

    virtual void BeginRender() const;
    virtual void Render() const;
    virtual void EndRender() const;

    void SetDrawMode(GLenum inputMode) {
        drawMode = inputMode;
    }

    GLint GetShaderProgram() const;
    const class ShaderProgram* GetShaderProgramRaw() const;

    decltype(auto) GetTotalVertices() const
    {
        return vertexPositions->size();
    }

    virtual void SetVertexPositions(std::unique_ptr<PositionArray> positions);
    virtual void SetVertexIndices(std::unique_ptr<IndexArray> indices);
    virtual void SetVertexNormals(std::unique_ptr<NormalArray> normals);
    virtual void SetVertexUV(std::unique_ptr<UVArray> uv);
    virtual void SetVertexColors(std::unique_ptr<ColorArray> colors);

protected:
    std::shared_ptr<class ShaderProgram> shader;

    GLuint vertexPositionBuffer;
    std::unique_ptr<PositionArray> vertexPositions;

    GLuint vertexIndexBuffer;
    std::unique_ptr<IndexArray> vertexIndices;

    GLuint vertexNormalBuffer;
    std::unique_ptr<NormalArray> vertexNormals;
    static glm::vec3 DEFAULT_NORMAL;

    GLuint vertexUVBuffer;
    std::unique_ptr<UVArray> vertexUV;
    static glm::vec2 DEFAULT_UV;

    GLuint vertexColorBuffer;
    std::unique_ptr<ColorArray> vertexColors;
    static glm::vec4 DEFAULT_COLOR;

    virtual void InitializeOpenGL();
    
    virtual void UpdateVertexPositions();
    virtual void UpdateVertexIndices();
    virtual void UpdateVertexNormals();
    virtual void UpdateVertexUV();
    virtual void UpdateVertexColors();

    virtual void CleanupVertexPositions();
    virtual void CleanupVertexIndices();
    virtual void CleanupVertexNormals();
    virtual void CleanupVertexUV();
    virtual void CleanupVertexColors();

    GLuint vao;
    GLenum drawMode;
};

#endif
