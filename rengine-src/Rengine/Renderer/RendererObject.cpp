#include "repch.hpp"
#include "RendererObject.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"
#include "Rengine/Renderer/Buffer.hpp"
#include "Rengine/Renderer/Material.hpp"

namespace Rengin

{

RendererObject::RendererObject()
{
}

void RendererObject::AddObj(const Ref<ObjManager> obj)
{
    ObjLists.push_back(obj);
}

void RendererObject::AddMethod(const std::string &name, Ref<RenderMethod> method)
{
    if(m_Methods.find(name) != m_Methods.end())   return;
    m_Methods[name] = method;
}

void RendererObject::DrawObject(const std::string& methodName,const std::vector<int>& ids,const EditorCamera& camera)
{
    m_Methods[methodName]->Render(ids,ObjLists,camera,lights);
}

void RendererObject::ComputePrt()
{
    for (int i = 0; i < ObjLists.size(); i++)
    {
        if(ObjLists[i]->UsePrt())
        {
            std::dynamic_pointer_cast<PRTObjManager>(ObjLists[i])->ComputeTransportSH(m_PrtType,this);
        }
    }
}

bool RendererObject::rayIntersect(const glm::vec3 &v, const glm::vec3 &wi)
{
    for (int i = 0; i < ObjLists.size(); i++)
    {
        if(std::dynamic_pointer_cast<PRTObjManager>(ObjLists[i])->hit(v,wi))   return true;
    }
    return false;
}

bool RendererObject::rayIntersect(const glm::vec3 &v, const glm::vec3 &wi, glm::vec3 &bary,std::vector<float>& value)
{
    bool flag = false;
    float t = 1e20;
    for (int i = 0; i < ObjLists.size(); i++)
    {
        if(std::dynamic_pointer_cast<PRTObjManager>(ObjLists[i])->hit(v,wi,bary,t,value))   flag = true;
    }
    return flag;
}

void RendererObject::GenerateLightBuffer()
{
    std::vector<float> lightsVer;
    int id = 0;
    for (auto &&obj : ObjLists)
    {
        lightsVer.insert(lightsVer.begin(),obj->GetLights().begin(),obj->GetLights().end());
    }
    lights.LightTriNum = lightsVer.size() / 18;
    lights.LightBuffer = StorageBuffer::Create(lightsVer.data(),lightsVer.size()); 
    lights.LightBuffer->Bind(3);
}

void RendererObject::GenerateMaterialBuffer()
{
    std::vector<Material> Materials;
    for (auto &&obj : ObjLists)
    {
        Materials.insert(Materials.begin(),obj->GetMaterials().begin(),obj->GetMaterials().end());
    }
    MaterialBuffer = StorageBuffer::Create(Materials.data(),Materials.size() * sizeof(Material)); 
    MaterialBuffer->Bind(4);
}

void RendererObject::GenerateTriangleBaseBuffer()
{
    std::vector<float> Vertex;
    std::vector<int> Indices;
    int Ioffset = 0,Voffset = 0;
    for (auto &&obj : ObjLists)
    {
        Vertex.insert(Vertex.begin(),obj->GetVertex().begin(),obj->GetVertex().end());
        for (int i = 0; i < obj->GetIndex().size(); i++)
        {
            Indices.push_back(Ioffset + obj->GetIndex()[i]);
        }
        Ioffset += obj->GetIndex().size();
    }
    VertexBuffer = StorageBuffer::Create(Vertex.data(),Vertex.size()); 
    VertexBuffer->Bind(0);

    IndexBuffer = StorageBuffer::Create(Indices.data(),Indices.size()); 
    IndexBuffer->Bind(2);
}

void RendererObject::GenerateTriangleMaterialIdBuffer()
{
    std::vector<int> MatId;
    int offset = 0,Moffset = 0;
    for (auto &&obj : ObjLists)
    {
        for (int i = 0; i < obj->GetMaterialID().size(); i++)
        {
            MatId.push_back(Moffset + obj->GetMaterialID()[i]);
        }
        // offset +=  obj->GetMaterialID().size();
        Moffset += obj->GetMaterialSize();
    }
    MaterialID = StorageBuffer::Create(MatId.data(),MatId.size()); 
    MaterialID->Bind(1);
}

} // namespace Rengin
