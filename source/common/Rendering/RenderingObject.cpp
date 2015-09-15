#include "common/Rendering/RenderingObject.h"
#include "common/Rendering/Shaders/ShaderProgram.h"

glm::vec3 RenderingObject::DEFAULT_NORMAL(0.f, 0.f, 1.f);
glm::vec2 RenderingObject::DEFAULT_UV(0.f, 0.f);
glm::vec4 RenderingObject::DEFAULT_COLOR(1.f, 1.f, 1.f, 1.f);

RenderingObject::RenderingObject(std::shared_ptr<ShaderProgram> inputShader,
        std::unique_ptr<PositionArray> inputPositions,
        std::unique_ptr<IndexArray> inputIndices,
        std::unique_ptr<NormalArray> inputNormals,
        std::unique_ptr<UVArray> inputUV,
        std::unique_ptr<ColorArray> inputColors):
    shader(std::move(inputShader)),
    vertexPositionBuffer(0), vertexPositions(std::move(inputPositions)), 
    vertexIndexBuffer(0), vertexIndices(std::move(inputIndices)),
    vertexNormalBuffer(0), vertexNormals(std::move(inputNormals)), 
    vertexUVBuffer(0), vertexUV(std::move(inputUV)), 
    vertexColorBuffer(0), vertexColors(std::move(inputColors)),
    drawMode(GL_TRIANGLES)
{
    assert(vertexPositions);
    InitializeOpenGL();
}

RenderingObject::~RenderingObject()
{
    OGL_CALL(glDeleteBuffers(1, &vertexPositionBuffer));
    OGL_CALL(glDeleteBuffers(1, &vertexIndexBuffer));
    OGL_CALL(glDeleteBuffers(1, &vertexNormalBuffer));
    OGL_CALL(glDeleteBuffers(1, &vertexUVBuffer));
    OGL_CALL(glDeleteBuffers(1, &vertexColorBuffer));
    OGL_CALL(glDeleteVertexArrays(1, &vao));
}

void RenderingObject::InitializeOpenGL()
{
    // Need to use the shader program here because 
    // the vertex attribute pointer needs to be associated with the shader.
    shader->StartUseShader();

    // Setup mesh data
    OGL_CALL(glGenVertexArrays(1, &vao));
    OGL_CALL(glBindVertexArray(vao));

    // Generate a multiple buffers to hold the various vertex attributes
    OGL_CALL(glGenBuffers(1, &vertexPositionBuffer));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer));
    OGL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * vertexPositions->size(), &(*vertexPositions.get())[0], GL_STATIC_DRAW));
    OGL_CALL(glVertexAttribPointer(static_cast<int>(VertexAttributePositions::Position), 4, GL_FLOAT, GL_FALSE, 0, 0));
    OGL_CALL(glEnableVertexAttribArray(static_cast<int>(VertexAttributePositions::Position)));

    if (vertexNormals) {
        if (vertexNormals->size() == vertexPositions->size()) {
            OGL_CALL(glGenBuffers(1, &vertexNormalBuffer));
            OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBuffer));
            OGL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexNormals->size(), &(*vertexNormals.get())[0], GL_STATIC_DRAW));
            OGL_CALL(glVertexAttribPointer(static_cast<int>(VertexAttributePositions::Normals), 3, GL_FLOAT, GL_FALSE, 0, 0));
            OGL_CALL(glEnableVertexAttribArray(static_cast<int>(VertexAttributePositions::Normals)));
        } else {
            std::cerr << "WARNING: Ignoring input vertex normals. It needs to have the same number of elements as the vertex positions vector." << std::endl;   
        }
    }
    
    if (!vertexNormalBuffer) {
        OGL_CALL(glVertexAttrib4fv(static_cast<int>(VertexAttributePositions::Normals), glm::value_ptr(DEFAULT_NORMAL)));
    }

    if (vertexUV) {
        if (vertexUV->size() == vertexPositions->size()) {
            OGL_CALL(glGenBuffers(1, &vertexUVBuffer));
            OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertexUVBuffer));
            OGL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertexUV->size(), &(*vertexUV.get())[0], GL_STATIC_DRAW));
            OGL_CALL(glVertexAttribPointer(static_cast<int>(VertexAttributePositions::Normals), 2, GL_FLOAT, GL_FALSE, 0, 0));
            OGL_CALL(glEnableVertexAttribArray(static_cast<int>(VertexAttributePositions::UV)));
        } else {
            std::cerr << "WARNING: Ignoring input vertex UVs. It needs to have the same number of elements as the vertex positions vector." << std::endl;   
        }
    }

    if (!vertexUVBuffer) {
        OGL_CALL(glVertexAttrib4fv(static_cast<int>(VertexAttributePositions::UV), glm::value_ptr(DEFAULT_UV)));
    }

    if (vertexColors) {
        if (vertexColors->size() == vertexPositions->size()) {
            OGL_CALL(glGenBuffers(1, &vertexColorBuffer));
            OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertexColorBuffer));
            OGL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * vertexColors->size(), &(*vertexColors.get())[0], GL_STATIC_DRAW));
            OGL_CALL(glVertexAttribPointer(static_cast<int>(VertexAttributePositions::Colors), 4, GL_FLOAT, GL_FALSE, 0, 0));
            OGL_CALL(glEnableVertexAttribArray(static_cast<int>(VertexAttributePositions::Colors)));
        } else {
            std::cerr << "WARNING: Ignoring input vertex colors. It needs to have the same number of elements as the vertex positions vector." << std::endl;   
        }
    }

    if (!vertexColorBuffer) {
        OGL_CALL(glVertexAttrib4fv(static_cast<int>(VertexAttributePositions::Colors), glm::value_ptr(DEFAULT_COLOR)));
    }

    // If vertex indices are used, then we can use an element array buffer to make use of more efficient rendering.
    if (vertexIndices) {
        OGL_CALL(glGenBuffers(1, &vertexIndexBuffer));
        OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer));
        OGL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * vertexIndices->size(), &(*vertexIndices.get())[0], GL_STATIC_DRAW));
    }

    // Unbind all the buffers to ensure that these buffers won't be modified elsewhereo
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    OGL_CALL(glBindVertexArray(0));

    shader->StopUseShader();
}

void RenderingObject::BeginRender() const
{
    shader->StartUseShader();
    OGL_CALL(glBindVertexArray(vao));
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer));
}

// TODO: Use the 'Instanced' versions of all these commands to batch these commands together.
void RenderingObject::Render() const
{
    // If we have an EBO (element array buffer) then use it, otherwise assume that the 
    // order of the vertices is already correct. Note that we assume that we are always
    // rendering triangles.
    if (vertexIndexBuffer) {
        OGL_CALL(glDrawElements(drawMode, vertexPositions->size(), GL_UNSIGNED_INT, (void*)0));
    } else {
        OGL_CALL(glDrawArrays(drawMode, 0, vertexPositions->size()));
    }
}

void RenderingObject::EndRender() const
{
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    OGL_CALL(glBindVertexArray(0));
    shader->StopUseShader();
}
