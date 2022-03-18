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

void ShadowMapMethod::Render()
{

}

template <typename... Args>
void ShadowMapMethod::AddResource(const Args&... resource) {
    static_assert(false);
}

template <> 
void ShadowMapMethod::AddResource<Ref<Shader>,Ref<Shader>>(const Ref<Shader>& base,const Ref<Shader>& shadow) {
    m_BaseShader = base;
    dynamic_cast<ShadowMapMethod*>(this)->m_ShadowShader = shadow;
}

template <> 
void ShadowMapMethod::AddResource<Ref<FrameBuffer>>(const Ref<FrameBuffer>& resource) {
    dynamic_cast<ShadowMapMethod*>(this)->m_ShadowMap = resource;
}

} // namespace Rengin
