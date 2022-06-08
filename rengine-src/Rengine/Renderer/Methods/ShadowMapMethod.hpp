#pragma once
#include "Rengine/Core/core.hpp"
#include "BaseMethod.hpp"
#include "../FrameBuffer.hpp"

namespace Rengin
{

class RE_API ShadowMapMethod : public RenderMethod
{
private:
    Ref<FrameBuffer> m_ShadowMap;
    Ref<FrameBuffer> m_MainFrame;
    Ref<Shader> m_ShadowShader;
public:
    ShadowMapMethod();
    virtual ~ShadowMapMethod();

    virtual void Render(const std::vector<int>& ids,const std::vector<Ref<ObjManager>>& ObjLists,const EditorCamera& camera,const Lights& lights) override;

    template<typename ...Args>
    void AddResource(const Args&... resource); 
};

} // namespace Rengin
