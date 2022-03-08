#pragma once
#include "Rengine/Core/core.hpp"
#include <string>
#include "Rengine/Renderer/VertexArray.hpp"

namespace Rengin
{
class ObjManager
{
private:
    Ref<VertexArray> m_VertexArray;
public:
    ObjManager(const std::string& path,const std::string& materialpath);
    ~ObjManager();
};

} // namespace Rengin
