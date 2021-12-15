#pragma once
#include "Rengine/core.hpp"
#include "RendererAPI.hpp"
#include "RenderCommand.hpp"
#include "VertexArray.hpp"

namespace Rengin
{

class RE_API Renderer
{
public:
    static void BeginScene();
    static void EndScene();
    static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
    inline static RendererAPI::API getRenderer(){return RendererAPI::getAPI();}
};

} // namespace Rengin
