#include "common/Rendering/RenderingObject.h"
#include "common/Rendering/Shaders/ShaderProgram.h"

glm::vec3 RenderingObject::DEFAULT_NORMAL(0.f, 0.f, 1.f);
glm::vec2 RenderingObject::DEFAULT_UV(0.f, 0.f);
glm::vec4 RenderingObject::DEFAULT_COLOR(1.f, 1.f, 1.f, 1.f);

RenderingObject::RenderingObject(std::shared_ptr<ShaderProgram> inputShader,
        std::unique_ptr<std::vector<glm::vec4>> inputPositions,
        std::unique_ptr<std::vector<unsigned int>> inputIndices,
        std::unique_ptr<std::vector<glm::vec3>> inputNormals,
        std::unique_ptr<std::vector<glm::vec2>> inputUV ,
        std::unique_ptr<std::vector<glm::vec4>> inputColors):
    shader(std::move(inputShader)),
    vertexPositionBuffer(0), vertexPositions(std::move(inputPositions)), 
    vertexIndexBuffer(0), vertexIndices(std::move(inputIndices)),
    vertexNormalBuffer(0), vertexNormals(std::move(inputNormals)), 
    vertexUVBuffer(0), vertexUV(std::move(inputUV)), 
    vertexColorBuffer(0), vertexColors(std::move(inputColors))  
{
    assert(vertexPositions);
    InitializeOpenGL();
}

RenderingObject::~RenderingObject()
{
    glDeleteBuffers(1, &vertexPositionBuffer);
    glDeleteBuffers(1, &vertexIndexBuffer);
    glDeleteBuffers(1, &vertexNormalBuffer);
    glDeleteBuffers(1, &vertexUVBuffer);
    glDeleteBuffers(1, &vertexColorBuffer);
    glDeleteVertexArrays(1, &vao);
}

void RenderingObject::InitializeOpenGL()
{
    // Need to use the shader program here because 
    // the vertex attribute pointer needs to be associated with the shader.
    shader->StartUseShader();

    // Setup mesh data
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate a multiple buffers to hold the various vertex attributes
    glGenBuffers(1, &vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * vertexPositions->size(), vertexPositions.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(static_cast<int>(VertexAttributePositions::Position), 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(static_cast<int>(VertexAttributePositions::Position));

    if (vertexNormals) {
        if (vertexNormals->size() == vertexPositions->size()) {
            glGenBuffers(1, &vertexNormalBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexNormals->size(), vertexNormals.get(), GL_STATIC_DRAW);
            glVertexAttribPointer(static_cast<int>(VertexAttributePositions::Normals), 3, GL_FLOAT, GL_TRUE, 0, 0);
        } else {
            std::cerr << "WARNING: Ignoring input vertex normals. It needs to have the same number of elements as the vertex positions vector." << std::endl;   
        }
    }
    
    if (!vertexNormalBuffer) {
        glVertexAttrib4fv(static_cast<int>(VertexAttributePositions::Normals), glm::value_ptr(DEFAULT_NORMAL));
    }
    glEnableVertexAttribArray(static_cast<int>(VertexAttributePositions::Normals));

    if (vertexUV) {
        if (vertexUV->size() == vertexPositions->size()) {
            glGenBuffers(1, &vertexUVBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexUVBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertexUV->size(), vertexUV.get(), GL_STATIC_DRAW);
            glVertexAttribPointer(static_cast<int>(VertexAttributePositions::Normals), 2, GL_FLOAT, GL_FALSE, 0, 0);
        } else {
            std::cerr << "WARNING: Ignoring input vertex UVs. It needs to have the same number of elements as the vertex positions vector." << std::endl;   
        }
    }

    if (!vertexUVBuffer) {
        glVertexAttrib4fv(static_cast<int>(VertexAttributePositions::UV), glm::value_ptr(DEFAULT_UV));
    }
    glEnableVertexAttribArray(static_cast<int>(VertexAttributePositions::UV));

    if (vertexColors) {
        if (vertexColors->size() == vertexPositions->size()) {
            glGenBuffers(1, &vertexColorBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexColorBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * vertexColors->size(), vertexColors.get(), GL_STATIC_DRAW);
            glVertexAttribPointer(static_cast<int>(VertexAttributePositions::Colors), 4, GL_FLOAT, GL_FALSE, 0, 0);
        } else {
            std::cerr << "WARNING: Ignoring input vertex colors. It needs to have the same number of elements as the vertex positions vector." << std::endl;   
        }
    }

    if (!vertexColorBuffer) {
        glVertexAttrib4fv(static_cast<int>(VertexAttributePositions::Colors), glm::value_ptr(DEFAULT_COLOR));
    }
    glEnableVertexAttribArray(static_cast<int>(VertexAttributePositions::Colors));

    // If vertex indices are used, then we can use an element array buffer to make use of more efficient rendering.
    if (vertexIndices) {
        glGenBuffers(1, &vertexIndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * vertexIndices->size(), vertexIndices.get(), GL_STATIC_DRAW);
    }

    // Unbind all the buffers to ensure that these buffers won't be modified elsewhereo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader->StopUseShader();
}

void RenderingObject::BeginRender() const
{
    shader->StartUseShader();
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
}

// TODO: Use the 'Instanced' versions of all these commands to batch these commands together.
void RenderingObject::Render() const
{
    // If we have an EBO (element array buffer) then use it, otherwise assume that the 
    // order of the vertices is already correct. Note that we assume that we are always
    // rendering triangles.
    if (vertexIndexBuffer) {
        glDrawElements(GL_TRIANGLES, vertexPositions->size(), GL_UNSIGNED_INT, (void*)0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, vertexPositions->size());
    }
}

void RenderingObject::EndRender() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    shader->StopUseShader();
}
