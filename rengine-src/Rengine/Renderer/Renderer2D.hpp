#pragma once
#include "Rengine/Core/core.hpp"
#include "OrthoGraphicsCamera.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"

namespace Rengin
{
class Renderer2D
{
private:

public:
    static void Init();
    static void Shutdown();
    static void OnWindowResized(uint32_t width ,uint32_t height);
    static void BeginScene(OrthoGraphicsCamera& camera);
    static void EndScene();
    static void Submit(const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray,const glm::mat4& transform = glm::mat4(1.0f));
    // inline static RendererAPI::API getRenderer(){return RendererAPI::getAPI();}

    //Primitives
    static void DrawQuad(const glm::vec2& position,const glm::vec2& size,const glm::vec4& color);
    static void DrawQuad(const glm::vec3& position,const glm::vec2& size,const glm::vec4& color);
    static void DrawQuad(const glm::vec2& position,const glm::vec2& size,const Ref<Texture>& texture);
    static void DrawQuad(const glm::vec3& position,const glm::vec2& size,const Ref<Texture>& texture);
};


} // namespace Rengin
