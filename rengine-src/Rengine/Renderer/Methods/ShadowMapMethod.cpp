#include "repch.hpp"
#include "ShadowMapMethod.hpp"

namespace Rengin
{

ShadowMapMethod::ShadowMapMethod()
{

}

ShadowMapMethod::~ShadowMapMethod()
{

}

void ShadowMapMethod::Render(const std::vector<int>& ids,const std::vector<ObjManager>& ObjLists,const EditorCamera& camera,const Lights& lights)
{

}

template <typename... Args>
void ShadowMapMethod::AddResource(const Args&... resource) {
    static_assert(false);
}

template <> 
void ShadowMapMethod::AddResource<Ref<Shader>,Ref<Shader>>(const Ref<Shader>& base,const Ref<Shader>& shadow) {
    m_BaseShader = base;
    m_ShadowShader = shadow;
}


template <> 
void ShadowMapMethod::AddResource<Ref<FrameBuffer>>(const Ref<FrameBuffer>& resource) {
    m_ShadowMap = resource;
}

} // namespace Rengin
