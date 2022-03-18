#pragma once
#include "Rengine/Core/core.hpp"
#include "BaseMethod.hpp"
#include "../FrameBuffer.hpp"

namespace Rengin
{

class ShadowMapMethod : public RenderMethod
{
private:
    Ref<FrameBuffer> m_ShadowMap;
public:
    ShadowMapMethod();
    virtual ~ShadowMapMethod();

    virtual void Render() override;

};

} // namespace Rengin
