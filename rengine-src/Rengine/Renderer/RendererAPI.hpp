#pragma once
#include "Rengine/Core/core.hpp"
#include <glm/glm.hpp>
#include "VertexArray.hpp"

namespace Rengin
{
class RE_API RendererAPI
{
public:
    enum class API
    {
        None = 0,
        OpenGL = 1,
        Direct3D = 2
    };
private:
    static API s_api;

public:
    virtual void Init() = 0;
    virtual void EnableAlpha() = 0;
    virtual void DisableAlpha() = 0;
    virtual void SetViewPort(uint32_t x,uint32_t y,uint32_t width,uint32_t height) = 0;
    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() = 0;
    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray,uint32_t count = 0) = 0;
    virtual void DrawLines(const Ref<VertexArray>& vertexArray,uint32_t count = 0) = 0;
    virtual void SetLineThickness(float thickness) = 0;

    inline static API getAPI(){return s_api;}
};


} // namespace Rengin
