#pragma once
#include "Rengine/core.hpp"
#include "VertexArray.hpp"
#include "RendererAPI.hpp"

namespace Rengin
{

class RE_API RenderCommand
{
private:
    static RendererAPI* s_api;

public:

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
    inline static void DrawIndex(const Ref<VertexArray>& vertexArray)
    {
        s_api->DrawIndexed(vertexArray);
    }
};

} // namespace Rengin
