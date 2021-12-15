#include "repch.hpp"
#include "Rengine/core.hpp"
#include "Buffer.hpp"
#include "Renderer.hpp"
#include "Rengine/Platform/OpenGL/OpenGLBuffer.hpp"

#ifdef RE_WIN_PLATFROM
    #include "Rengine/Platform/Direct3D/Direct3DBuffer.hpp"
#endif

namespace Rengin
{

VertexBuffer* VertexBuffer::Create(float *vertices,uint32_t size)
{
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        return nullptr;
        break;
    case RendererAPI::API::OpenGL :
        return new OpenGLVertexBuffer(vertices,size);
        break;
    case RendererAPI::API::Direct3D :
        RE_CORE_ASSERT(false,"not support for DirectX3D Render API");
        return nullptr;
        break;
    }
    RE_CORE_ASSERT(false,"Unknown RendererAPI!");
    return nullptr;
}

IndexBuffer* IndexBuffer::Create(uint32_t *indices,uint32_t size)
{    
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        return nullptr;
        break;
    case RendererAPI::API::OpenGL :
        return new OpenGLIndexBuffer(indices,size);
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

