#pragma once
#include "Rengine/core.hpp"
#include <glm/glm.hpp>
#include "VertexArray.hpp"

namespace Rengin
{
class RE_API RendererAPI
{
private:
    static API s_api;
public:
    enum class API
    {
        None = 0,
        OpenGL = 1,
        Direct3D = 2
    };
public:
    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() = 0;
    virtual void DrawIndexed(std::shared_ptr<VertexArray>& vertexArray) = 0;

    inline static API getAPI()const {return s_api;}
};


} // namespace Rengin