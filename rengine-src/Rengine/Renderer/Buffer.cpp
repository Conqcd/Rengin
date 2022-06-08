#include "repch.hpp"
#include "Rengine/Core/core.hpp"
#include "Buffer.hpp"
#include "Renderer.hpp"
#include "Rengine/Platform/OpenGL/OpenGLBuffer.hpp"

#ifdef RE_WIN_PLATFROM
    #include "Rengine/Platform/Direct3D/Direct3DBuffer.hpp"
#endif

namespace Rengin
{

Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
{
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        return nullptr;
    case RendererAPI::API::OpenGL :
        return CreateRef<OpenGLVertexBuffer>(size);
    case RendererAPI::API::Direct3D :
        RE_CORE_ASSERT(false,"not support for DirectX3D Render API");
        return nullptr;
    }
    RE_CORE_ASSERT(false,"Unknown RendererAPI!");
    return nullptr;
}

Ref<VertexBuffer> VertexBuffer::Create(void *vertices,uint32_t size)
{
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        return nullptr;
    case RendererAPI::API::OpenGL :
        return CreateRef<OpenGLVertexBuffer>(vertices,size);
    case RendererAPI::API::Direct3D :
        RE_CORE_ASSERT(false,"not support for DirectX3D Render API");
        return nullptr;
    }
    RE_CORE_ASSERT(false,"Unknown RendererAPI!");
    return nullptr;
}

Ref<StorageBuffer> StorageBuffer::Create(uint32_t size)
{
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        return nullptr;
    case RendererAPI::API::OpenGL :
        return CreateRef<OpenGLStorageBuffer>(size);
    case RendererAPI::API::Direct3D :
        RE_CORE_ASSERT(false,"not support for DirectX3D Render API");
        return nullptr;
    }
    RE_CORE_ASSERT(false,"Unknown RendererAPI!");
    return nullptr;
}

Ref<StorageBuffer> StorageBuffer::Create(void *vertices,uint32_t size)
{
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        return nullptr;
    case RendererAPI::API::OpenGL :
        return CreateRef<OpenGLStorageBuffer>(vertices,size);
    case RendererAPI::API::Direct3D :
        RE_CORE_ASSERT(false,"not support for DirectX3D Render API");
        return nullptr;
    }
    RE_CORE_ASSERT(false,"Unknown RendererAPI!");
    return nullptr;
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices,uint32_t size)
{    
    switch (Renderer::getRenderer())
    {
    case RendererAPI::API::None :
        RE_CORE_ASSERT(false,"not support for No Render API");
        return nullptr;
    case RendererAPI::API::OpenGL :
        return CreateRef<OpenGLIndexBuffer>(indices,size);
    case RendererAPI::API::Direct3D :
        RE_CORE_ASSERT(false,"not support for DirectX3D Render API");
        return nullptr;
    }
    RE_CORE_ASSERT(false,"Unknown RendererAPI!");
    return nullptr;
}
} // namespace Rengin

