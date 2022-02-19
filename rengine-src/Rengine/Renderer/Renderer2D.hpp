#pragma once
#include "Rengine/Core/core.hpp"
#include "OrthoGraphicsCamera.hpp"
#include "Camera.hpp"
#include "EditorCamera.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "SubTexture2D.hpp"

namespace Rengin
{
class Renderer2D
{
private:
    static void StartBatch();

public:
    static void Init();
    static void Shutdown();
    static void OnWindowResized(uint32_t width ,uint32_t height);

    static void BeginScene(const Camera& camera,const glm::mat4& transform);
    static void BeginScene(const OrthoGraphicsCamera& camera); // TODO: Remove
    static void BeginScene(const EditorCamera& camera); 
    static void EndScene();
    static void Flush();

    //Primitives
    static void DrawQuad(const glm::vec2& position,const glm::vec2& size,const glm::vec4& color);
    static void DrawQuad(const glm::vec3& position,const glm::vec2& size,const glm::vec4& color);
    static void DrawQuad(const glm::vec2& position,const glm::vec2& size,const Ref<Texture>& texture,float tile_factor = 1.0,const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec3& position,const glm::vec2& size,const Ref<Texture>& texture,float tile_factor = 1.0,const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec2& position,const glm::vec2& size,const Ref<SubTexture2D>& subtexture,float tile_factor = 1.0,const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec3& position,const glm::vec2& size,const Ref<SubTexture2D>& subtexture,float tile_factor = 1.0,const glm::vec4& tintColor = glm::vec4(1.0f));

    static void DrawRotatedQuad(const glm::vec2& position,const glm::vec2& size,float rotation,const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec3& position,const glm::vec2& size,float rotation,const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec2& position,const glm::vec2& size,float rotation,const Ref<Texture>& texture,float tile_factor = 1.0,const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawRotatedQuad(const glm::vec3& position,const glm::vec2& size,float rotation,const Ref<Texture>& texture,float tile_factor = 1.0,const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawRotatedQuad(const glm::vec2& position,const glm::vec2& size,float rotation,const Ref<SubTexture2D>& subtexture,float tile_factor = 1.0,const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawRotatedQuad(const glm::vec3& position,const glm::vec2& size,float rotation,const Ref<SubTexture2D>& subtexture,float tile_factor = 1.0,const glm::vec4& tintColor = glm::vec4(1.0f));

    struct Statistic
    {
        uint32_t DrawCall = 0;
        uint32_t QuadCount = 0;

        uint32_t GetTotalVertexCount()const {return QuadCount * 4;}
        uint32_t GetTotalIndexCount()const {return QuadCount * 6;}
    };
    static Statistic getStats();
    static void resetStats();

private:
    static void FlushAndReset();
};


} // namespace Rengin
