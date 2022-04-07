#pragma once
#include "Rengine/Core/core.hpp"
#include "BaseMethod.hpp"
#include "../FrameBuffer.hpp"

namespace Rengin
{

class SSRMethod : public RenderMethod
{
private:
    Ref<FrameBuffer> m_ShadowMap;
    Ref<FrameBuffer> m_GBuffer;
    Ref<FrameBuffer> m_MainFrame;
    Ref<Shader> m_GBufferShader;
    Ref<Shader> m_SSRShader;
    Ref<Shader> m_ShadowShader;
public:
    SSRMethod();
    virtual ~SSRMethod();

    virtual void Render(const std::vector<int>& ids,const std::vector<Ref<ObjManager>>& ObjLists,const EditorCamera& camera,const Lights& lights) override;

    template<typename ...Args>
    void AddResource(const Args&... resource); 
};

} // namespace Rengin
