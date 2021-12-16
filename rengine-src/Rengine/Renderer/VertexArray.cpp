#include "repch.hpp"
#include "VertexArray.hpp"
#include "Renderer.hpp"
#include "Rengine/Platform/OpenGL/OpenGLVertexArray.hpp"

namespace Rengin
{

VertexArray* VertexArray::Create()
{
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        return nullptr;
        break;
    case RendererAPI::API::OpenGL :
        return new OpenGLVertexArray();
        break;
    case RendererAPI::API::Direct3D :
        RE_CORE_ASSERT(false,"not support for DirectX3D Render API");
        return nullptr;
        break;
    }
    RE_CORE_ASSERT(false,"Unknown RendererAPI!");
    return nullptr;
}
} // namespace Rengin
