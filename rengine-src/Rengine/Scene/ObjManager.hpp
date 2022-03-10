#pragma once
#include <string>
#include "Rengine/Core/core.hpp"
#include "Rengine/Renderer/VertexArray.hpp"

namespace Rengin
{
class ObjManager
{
private:
    Ref<VertexArray> m_VertexArray;
public:
    ObjManager(const std::string& path,const std::string& materialpath);
    const Ref<VertexArray> GetVertexArray() const {return m_VertexArray;}
    ~ObjManager();
};

} // namespace Rengin
