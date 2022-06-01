#include "repch.hpp"
#include "KullaCountyMethod.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"

namespace Rengin
{
 
KullaCountyMethod::KullaCountyMethod(/* args */)
{
    m_BRDFLUT = Texture2D::Create("../../../Rengine-Editor/assets/LUT/GGX_E_LUT.png");
    m_Eavg = Texture2D::Create("../../../Rengine-Editor/assets/LUT/GGX_Eavg_LUT.png");
}

KullaCountyMethod::~KullaCountyMethod()
{

}

void KullaCountyMethod::Render(const std::vector<int>& ids,const std::vector<Ref<ObjManager>>& ObjLists,const EditorCamera& camera,const Lights& lights)
{
    m_BaseShader->Bind();
    m_BaseShader->SetUniformMat4("u_View", camera.GetViewMatrix());
    m_BaseShader->SetUniformMat4("u_Projection", camera.getProjection());
    m_BaseShader->SetUniformFloat3("u_LightDir", lights.Direction);
    m_BaseShader->SetUniformFloat3("u_LightPos", lights.LightPos);
    m_BaseShader->SetUniformFloat3("u_CameraPos", camera.GetPosition());
    m_BaseShader->SetUniformFloat3("u_LightRadiance", lights.LightIntensity);

    m_BRDFLUT->Bind(0);
    m_Eavg->Bind(1);
    m_BaseShader->SetUniformInt("u_BRDFLut", 0);
    m_BaseShader->SetUniformInt("u_EavgLut", 1);
    int tidx = 2;
    float Metalics[5] = {0.2,0.4,0.6,0.8,1.0};
    float Roughnesses[5] = {0.2,0.4,0.6,0.8,1.0};
    for (int i = 0; i < ids.size(); i++)
    {
        m_BaseShader->SetUniformInt("u_Entity", ids[i]);
        for (int j = 0; j < ObjLists[ids[i]]->GetVertexArraySize(); j++)
        {
            m_BaseShader->SetUniformInt("u_AlbedoMap", tidx++);
            ObjLists[ids[i]]->BindTexture(j, tidx);
            m_BaseShader->SetUniformMat4("u_Transform",ObjLists[ids[i]]->GetTransform());
            m_BaseShader->SetUniformFloat("u_Metallic", Metalics[i]);
            m_BaseShader->SetUniformFloat("u_Roughness", Roughnesses[i]);
            m_BaseShader->SetUniformFloat3("u_Kd", ObjLists[ids[i]]->GetMaterial(j).Kd);

            RenderCommand::DrawIndex(ObjLists[ids[i]]->GetVertexArray(j));
        }
    }
}

template <typename... Args>
void KullaCountyMethod::AddResource(const Args&... resource) 
{
    static_assert(false);
}

template <> 
void KullaCountyMethod::AddResource<Ref<Shader>>(const Ref<Shader>& base) {
    m_BaseShader = base;
}
} // namespace Rengin
