#include "repch.hpp"
#include "PRTMethod.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"

namespace Rengin
{
 
PRTMethod::PRTMethod(/* args */)
{

}

PRTMethod::~PRTMethod()
{

}

void PRTMethod::Render(const std::vector<int>& ids,const std::vector<ObjManager>& ObjLists,const EditorCamera& camera,const Lights& lights)
{
    m_SkyBoxShader->Bind();

    m_BaseShader->Bind();
    m_BaseShader->SetUniformMat4("u_View", camera.GetViewMatrix());
    m_BaseShader->SetUniformMat4("u_Projection", camera.getProjection());
    for (int i = 0; i < ids.size(); i++)
    {
        m_BaseShader->SetUniformInt("u_Entity", ids[i]);
        for (int j = 0; j < ObjLists[ids[i]].GetVertexArraySize(); j++)
        {
            m_BaseShader->SetUniformMat4("u_Transform",ObjLists[ids[i]].GetTransform());

            RenderCommand::DrawIndex(ObjLists[ids[i]].GetVertexArray(j));
        }
    }
}

template <typename... Args>
void PRTMethod::AddResource(const Args&... resource) {
    static_assert(false);
}

template <> 
void PRTMethod::AddResource<Ref<Shader>,Ref<Shader>>(const Ref<Shader>& base,const Ref<Shader>& SkyBoxShader) {
    m_BaseShader = base;
    m_SkyBoxShader = SkyBoxShader;
}
} // namespace Rengin
