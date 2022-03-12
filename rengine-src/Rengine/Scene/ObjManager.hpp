#pragma once
#include <string>
#include "Rengine/Core/core.hpp"
#include <vector>
#include "Rengine/Renderer/VertexArray.hpp"
#include "Rengine/Renderer/Texture.hpp"

namespace Rengin
{
class ObjManager
{
private:
    std::vector<Ref<VertexArray>> m_VertexArrays;
    std::vector<Ref<Texture2D>> m_Textures;
public:
    ObjManager(const std::string& path,const std::string& materialpath);
    const Ref<VertexArray> GetVertexArray(int i) const {return m_VertexArrays[i];}
    size_t GetVertexArraySize() const {return m_VertexArrays.size();}
    void BindTexture(int slot = 0);
    ~ObjManager();
};

} // namespace Rengin
