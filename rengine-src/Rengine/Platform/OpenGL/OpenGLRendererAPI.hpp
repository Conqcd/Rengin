#pragma once
#include "Rengine/Renderer/RendererAPI.hpp"
#include "Rengine/Renderer/VertexArray.hpp"

namespace Rengin
{
class RE_API OpenGLRendererAPI : public RendererAPI
{
public:
    virtual void Init() override;
    
    virtual void SetClearColor(const glm::vec4& color) override;
    virtual void Clear() override;
    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
};



} // namespace Rengin
