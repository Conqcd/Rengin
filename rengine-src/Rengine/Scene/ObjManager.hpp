#pragma once
#include <glm/glm.hpp>
#include "Rengine/Core/core.hpp"
#include <string>
#include <vector>
#include "Rengine/Renderer/VertexArray.hpp"
#include "Rengine/Renderer/Material.hpp"
#include "Rengine/Renderer/Texture.hpp"

namespace Rengin
{
class ObjManager
{
private:
    std::vector<Ref<VertexArray>> m_VertexArrays;
    std::vector<Material> m_Materials;
    std::vector<Ref<Texture2D>> m_Textures;
    glm::mat4 m_transform;
public:
    ObjManager(const std::string& path,const std::string& materialpath,const glm::mat4& transform);
    const Ref<VertexArray> GetVertexArray(int i) const {return m_VertexArrays[i];}
    size_t GetVertexArraySize() const {return m_VertexArrays.size();}
    const Material& GetMaterial(int i) const {return m_Materials[i];}
    const glm::mat4& GetTransform() const {return m_transform;}
    void BindTexture(int slot = 0) const;
    ~ObjManager();
};
} // namespace Rengin
