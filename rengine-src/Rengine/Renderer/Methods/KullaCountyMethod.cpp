#include "repch.hpp"
#include "KullaCountyMethod.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"

namespace Rengin
{
 
KullaCountyMethod::KullaCountyMethod(/* args */)
{

}

KullaCountyMethod::~KullaCountyMethod()
{

}

void KullaCountyMethod::Render(const std::vector<int>& ids,const std::vector<Ref<ObjManager>>& ObjLists,const EditorCamera& camera,const Lights& lights)
{
    m_BaseShader->Bind();
    m_BaseShader->SetUniformMat4("u_View", camera.GetViewMatrix());
    m_BaseShader->SetUniformMat4("u_Projection", camera.getProjection());
    for (int i = 0; i < ids.size(); i++)
    {
        m_BaseShader->SetUniformInt("u_Entity", ids[i]);
        for (int j = 0; j < ObjLists[ids[i]]->GetVertexArraySize(); j++)
        {
            m_BaseShader->SetUniformMat4("u_Transform",ObjLists[ids[i]]->GetTransform());

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
void KullaCountyMethod::AddResource<Ref<Shader>,Ref<Shader>>(const Ref<Shader>& base) {
    m_BaseShader = base;
}
} // namespace Rengin
