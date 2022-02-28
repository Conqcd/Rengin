#pragma once
#include "Rengine/Core/core.hpp"
#include "VertexArray.hpp"
#include "RendererAPI.hpp"

namespace Rengin
{

class RE_API RenderCommand
{
private:
    static RendererAPI* s_api;

public:

    inline static void SetViewPort(uint32_t x,uint32_t y,uint32_t width,uint32_t height)
    {
        s_api->SetViewPort(x,y,width,height);
    }
    inline static void Init()
    {
        s_api->Init();
    }
    inline static void SetClearColor(const glm::vec4& color)
    {
        s_api->SetClearColor(color);
    }
    inline static void Clear()
    {
        s_api->Clear();
    }
    inline static void DrawIndex(const Ref<VertexArray>& vertexArray,uint32_t count = 0)
    {
        s_api->DrawIndexed(vertexArray,count);
    }
    inline static void DrawLines(const Ref<VertexArray>& vertexArray,uint32_t count = 0)
    {
        s_api->DrawLines(vertexArray,count);
    }
};

} // namespace Rengin
