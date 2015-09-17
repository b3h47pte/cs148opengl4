#pragma once

#ifndef __MESH_LOADER__
#define __MESH_LOADER__

#include "common/common.h"

class RenderingObject;
class ShaderProgram;

namespace MeshLoader
{

std::vector<std::shared_ptr<RenderingObject>> LoadMesh(std::shared_ptr<ShaderProgram> inputShader, const std::string& filename);

}

#endif
