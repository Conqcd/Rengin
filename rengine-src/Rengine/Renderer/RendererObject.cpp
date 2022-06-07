#include "repch.hpp"
#include "RendererObject.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"
#include "Rengine/Renderer/Buffer.hpp"

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
    lights.LightTriNum = lightsVer.size() / 3;
    lights.LightBuffer = StorageBuffer::Create(lightsVer.data(),lightsVer.size()); 
}

} // namespace Rengin
