#pragma once
#include "Rengine/Core/core.hpp"
#include <vector>
#include <glm/glm.hpp>

namespace Rengin
{

struct Node
{
    glm::vec3 bmin,bmax;
    int lchild,rchild;
    int lleaf,rleaf;
};

void ComputeBVHandMorton(std::vector<int> &indices,std::vector<int> &newindices,const std::vector<float> &vertices,std::vector<float> &leafNodes,std::vector<Node>& internalNodes);

} // namespace Rengin
