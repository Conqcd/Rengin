#pragma once
#include <glm/glm.hpp>
#include "Texture.hpp"


namespace Rengin
{
class SubTexture2D 
{
private:
    Ref<Texture2D> m_texture;
    glm::vec2 m_texCoords[4];
public:
    SubTexture2D(const Ref<Texture2D>& texture,const glm::vec2 min,const glm::vec2 max);
    const glm::vec2* getTexCoords()const;
    const Ref<Texture2D> getTexture()const;

    static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture,const glm::vec2& coords,const glm::vec2& cellSize,const glm::vec2& spriteSize = {1.0f ,1.0f});
};

} // namespace Rengin