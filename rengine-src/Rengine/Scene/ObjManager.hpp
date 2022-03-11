#pragma once
#include "Rengine/Core/core.hpp"
#include <string>
#include <vector>
#include "Rengine/Renderer/VertexArray.hpp"
#include "Rengine/Renderer/Texture.hpp"

namespace Rengin
{
class ObjManager
{
private:
    Ref<VertexArray> m_VertexArray;
    std::vector<Ref<Texture2D>> m_Textures;
public:
    ObjManager(const std::string& path,const std::string& materialpath);
    const Ref<VertexArray> GetVertexArray() const {return m_VertexArray;}
    void BindTexture(int slot = 0);
    ~ObjManager();
};

} // namespace Rengin
