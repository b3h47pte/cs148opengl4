#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Rendering/RenderingObject.h"
#include <cmath>

namespace PrimitiveCreator 
{
void AddTriangleIndices(const glm::uvec3& triangle, RenderingObject::IndexArray& indices)
{
    for (int i = 0; i < 3; ++i) {
        indices.push_back(triangle[i]);
    }
}

std::shared_ptr<RenderingObject> CreateCube(std::shared_ptr<ShaderProgram> inputShader, float sideLength)
{
    return nullptr;
}

std::shared_ptr<RenderingObject> CreateIcoSphere(std::shared_ptr<ShaderProgram> inputShader, float radius, int refinementSteps)
{
    // First generate an icosahedron and then subdivide it to get the final sphere.
    // More specifics in generating the icosahedron can be found at: 
    //  http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
    //  https://en.wikipedia.org/wiki/Regular_icosahedron
    const float gr = (1.f + std::sqrtf(5.f)) * 0.5f;

    std::unique_ptr<RenderingObject::PositionArray> vertexPositions = std::make_unique<RenderingObject::PositionArray>();
    // An Icohedron's vertices can be placed onto three orthogonal rectangles (check the links for pictures!)
    // Note that the icosahedron code is based off of the implementation by Andreas Kahler.
    vertexPositions->emplace_back(-1.f, gr, 0.f, 1.f);
    vertexPositions->emplace_back(1.f, gr, 0.f, 1.f);
    vertexPositions->emplace_back(-1.f, -gr, 0.f, 1.f);
    vertexPositions->emplace_back(1.f, -gr, 0.f, 1.f);

    vertexPositions->emplace_back(0.f, -1.f, gr, 1.f);
    vertexPositions->emplace_back(0.f, 1.f, gr, 1.f);
    vertexPositions->emplace_back(0.f, -1.f, -gr, 1.f);
    vertexPositions->emplace_back(0.f, 1.f, -gr, 1.f);

    vertexPositions->emplace_back(gr, 0.f, -1.f, 1.f);
    vertexPositions->emplace_back(gr, 0.f, 1.f, 1.f);
    vertexPositions->emplace_back(-gr, 0.f, -1.f, 1.f);
    vertexPositions->emplace_back(-gr, 0.f, 1.f, 1.f);

    // Force every vertex to be on the sphere of radius 'radius'
    std::for_each(vertexPositions->begin(), vertexPositions->end(),
        [=](auto& position) {
            position = glm::vec4(glm::normalize(glm::vec3(position)) * radius, 1.f);
        }
    );

    RenderingObject::IndexArray vertexIndices = {
        0, 11, 5,
        0, 5, 1,
        0, 1, 7,
        0, 7, 10,
        0, 10, 11,

        1, 5, 9,
        5, 11, 4,
        11, 10, 2,
        10, 7, 6,
        7, 1, 8,

        3, 9, 4,
        3, 4, 2,
        3, 2, 6,
        3, 6, 8,
        3, 8, 9,

        4, 9, 5,
        2, 4, 11,
        6, 2, 10,
        8, 6, 7,
        9, 8, 1
    };

    // Subdivide each triangle into 4 different triangles by adding the midpoints of on each edge. Force the one vertex to be on the sphere of the given radius.
    for (int i = 0; i < refinementSteps; ++i) {
        const auto totalTriangles = vertexIndices.size() / 3;

        RenderingObject::IndexArray newVertexIndices;
        newVertexIndices.reserve(totalTriangles * 4);
        for (auto t = 0; t < totalTriangles; ++t) {
            const int triangleOffset = t * 3;
            const glm::vec4 midpointA = glm::vec4(glm::normalize(glm::vec3(vertexPositions->at(vertexIndices[triangleOffset]) + vertexPositions->at(vertexIndices[triangleOffset  + 1])) / 2.f) * radius, 1.f);
            const glm::vec4 midpointB = glm::vec4(glm::normalize(glm::vec3(vertexPositions->at(vertexIndices[triangleOffset + 1]) + vertexPositions->at(vertexIndices[triangleOffset  + 2])) / 2.f) * radius, 1.f);
            const glm::vec4 midpointC = glm::vec4(glm::normalize(glm::vec3(vertexPositions->at(vertexIndices[triangleOffset + 2]) + vertexPositions->at(vertexIndices[triangleOffset])) / 2.f) * radius, 1.f);

            const unsigned int aIndex = static_cast<unsigned int>(vertexPositions->size());
            const unsigned int bIndex = static_cast<unsigned int>(vertexPositions->size() + 1);
            const unsigned int cIndex = static_cast<unsigned int>(vertexPositions->size() + 2);

            AddTriangleIndices(glm::uvec3(vertexIndices[triangleOffset], aIndex, cIndex), newVertexIndices);
            AddTriangleIndices(glm::uvec3(vertexIndices[triangleOffset + 1], bIndex, aIndex), newVertexIndices);
            AddTriangleIndices(glm::uvec3(vertexIndices[triangleOffset + 2], cIndex, bIndex), newVertexIndices);
            AddTriangleIndices(glm::uvec3(aIndex, bIndex, cIndex), newVertexIndices);

            vertexPositions->push_back(midpointA);
            vertexPositions->push_back(midpointB);
            vertexPositions->push_back(midpointC);
        }
        vertexIndices = std::move(newVertexIndices);
    }

    std::shared_ptr<RenderingObject> newObj = std::make_shared<RenderingObject>(std::move(inputShader),
        std::move(vertexPositions), 
        std::make_unique<RenderingObject::IndexArray>(std::move(vertexIndices))
    );
            
    return newObj;
}
}
