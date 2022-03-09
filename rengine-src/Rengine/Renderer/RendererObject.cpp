#include "repch.hpp"
#include "RendererObject.hpp"
#include "Rengine/Renderer/RenderCommand.hpp"

namespace Rengin
{

void RendererObject::AddObj(const ObjManager& obj)
{
    ObjLists.push_back(obj);
}

void RendererObject::DrawObject(uint32_t id,Ref<Shader> shader)
{
    shader->Bind();

    RenderCommand::DrawIndex(ObjLists[id].GetVertexArray());
}

} // namespace Rengin
