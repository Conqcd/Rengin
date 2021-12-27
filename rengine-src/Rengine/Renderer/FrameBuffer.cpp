#include "repch.hpp"
#include "FrameBuffer.hpp"
#include "Renderer.hpp"
#include "Rengine/Platform/OpenGL/OpenGLFrameBuffer.hpp"

namespace Rengin
{

Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
{
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        return nullptr;
    case RendererAPI::API::OpenGL :
        return CreateRef<OpenGLFrameBuffer>(spec);
    case RendererAPI::API::Direct3D :
        RE_CORE_ASSERT(false,"not support for DirectX3D Render API");
        return nullptr;
    }
    RE_CORE_ASSERT(false,"Unknown RendererAPI!");
    return nullptr;
}
   
} // namespace Rengin
