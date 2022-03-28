#pragma once
#include <glm/glm.hpp>
#include "Rengine/Core/core.hpp"
#include <string>
#include <vector>
#include "Rengine/Renderer/VertexArray.hpp"
#include "Rengine/Renderer/Material.hpp"
#include "Rengine/Renderer/Texture.hpp"
#include "tiny_obj_loader.hpp"

namespace Rengin
{
class ObjManager
{
protected:
    std::vector<Ref<VertexArray>> m_VertexArrays;
    std::vector<Material> m_Materials;
    std::vector<Ref<Texture2D>> m_Textures;
    glm::mat4 m_transform;
    bool isPrt = false;
public:
    ObjManager() = default;
    ObjManager(const std::string& path,const std::string& material_path,const glm::mat4& transform);
    const Ref<VertexArray> GetVertexArray(int i) const {return m_VertexArrays[i];}
    size_t GetVertexArraySize() const {return m_VertexArrays.size();}
    const Material& GetMaterial(int i) const {return m_Materials[i];}
    const glm::mat4& GetTransform() const {return m_transform;}
    void BindTexture(int id,int slot = 0) const;
    virtual ~ObjManager();
    bool isPrt() const {return isPrt;}
};

class PRTObjManager :public ObjManager
{
private:
    std::vector<tinyobj::real_t> m_Vertices;
public:
    PRTObjManager(const std::string& path,const std::string& material_path,const glm::mat4& transform);
    virtual ~PRTObjManager() = default;
    void AddPRTVertex(const std::string& prtpath);
    void ComputeTransportSH(const std::string& path);
};


} // namespace Rengin
