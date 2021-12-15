#pragma once
#include "Rengine/Renderer/RendererAPI.hpp"
#include "Rengine/Renderer/VertexArray.hpp"

namespace Rengin
{
class RE_API OpenGLRendererAPI : public RendererAPI
{
private:
    /* data */
public:
    
    virtual void SetClearColor(const glm::vec4& color) override;
    virtual void Clear() override;
    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
};



} // namespace Rengin
