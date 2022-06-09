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

class RendererObject;

enum class PRTType
{
    Unshadowed,
    Shadow,
    Interreflection
};
class RE_API ObjManager
{
protected:
    std::vector<Ref<VertexArray>> m_VertexArrays;
    std::vector<Material> m_Materials;
    std::vector<int> m_MaterialId;
    std::vector<Ref<Texture2D>> m_Textures;
    std::vector<float> m_Vertices;
    std::vector<int> m_Indices;
    std::vector<float> m_LightsVertex;
    std::vector<int> m_LightsIndex;
    glm::mat4 m_transform;
    bool isPrt = false;
    std::string m_Path;
    int m_SampleCount = 100;
    int m_Bounce = 1;
public:
    ObjManager() = default;
    ObjManager(const std::string& path,const std::string& material_path,const glm::mat4& transform);
    const Ref<VertexArray> GetVertexArray(int i) const {return m_VertexArrays[i];}
    size_t GetVertexArraySize() const {return m_VertexArrays.size();}
    size_t GetMaterialSize() const {return m_Materials.size();}
    size_t GetTriangleNums() const {return m_Indices.size() / 3;}

    const Material& GetMaterial(int i) const {return m_Materials[i];}
    const std::vector<Material>& GetMaterials() const {return m_Materials;}
    const std::vector<int>& GetMaterialID() const {return m_MaterialId;}
    const std::vector<float>& GetVertex() const {return m_Vertices;}
    const std::vector<int>& GetIndex() const {return m_Indices;}

    const glm::mat4& GetTransform() const {return m_transform;}

    const const std::vector<float>& GetLights() const {return m_LightsVertex;}
    const const std::vector<int>& GetLightId() const {return m_LightsIndex;}
    void BindTexture(int id,int slot = 0) const;
    virtual ~ObjManager();
    bool UsePrt() const {return isPrt;}
};

class RE_API PRTObjManager :public ObjManager
{
private:
    std::vector<tinyobj::real_t> m_Vertices,m_Normals;
    std::vector<float> m_Transport;
    std::vector<std::vector<uint32_t>> m_IndicesList;
    bool hasTransportSH = false;
public:
    PRTObjManager(const std::string& path,const std::string& material_path,const glm::mat4& transform);
    virtual ~PRTObjManager() = default;
    void AddPRTVertex(const std::string& prtpath);
    void ComputeTransportSH(PRTType type,RendererObject* total);
    bool hit(const glm::vec3& v,const glm::vec3& wi);
    bool hit(const glm::vec3& v,const glm::vec3& wi,glm::vec3& bary,float &t,std::vector<float>& value);
};


} // namespace Rengin
