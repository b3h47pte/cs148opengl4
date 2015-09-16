#pragma once

#ifndef __PRIMITIVE_CREATOR__
#define __PRIMITIVE_CREATOR__

#include "common/common.h"
#include "common/Rendering/RenderingObject.h"

namespace PrimitiveCreator
{

//
// Utility
//
void AddTriangleIndices(const glm::uvec3& triangle, RenderingObject::IndexArray& indices);

//
// Primitive Generation
//

std::shared_ptr<RenderingObject> CreateCube(std::shared_ptr<ShaderProgram> inputShader, float sideLength);
std::shared_ptr<RenderingObject> CreateIcoSphere(std::shared_ptr<ShaderProgram> inputShader, float radius, int refinementSteps = 0);

}

#endif
