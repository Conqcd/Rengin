#pragma once
#include "Rengine/Renderer/RendererAPI.hpp"
#include "Rengine/Renderer/VertexArray.hpp"

namespace Rengin
{
class RE_API OpenGLRendererAPI : public RendererAPI
{
public:
    virtual void Init() override;
    virtual void SetViewPort(uint32_t x,uint32_t y,uint32_t width,uint32_t height) override;
    
    virtual void SetClearColor(const glm::vec4& color) override;
    virtual void Clear() override;
    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray,uint32_t count = 0) override;
    virtual void DrawLines(const Ref<VertexArray>& vertexArray,uint32_t count = 0) override;
    virtual void SetLineThickness(float thickness) override;
};



} // namespace Rengin
