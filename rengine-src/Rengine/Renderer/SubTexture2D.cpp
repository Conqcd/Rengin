#include "repch.hpp"
#include "SubTexture2D.hpp"

namespace Rengin
{

SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture,const glm::vec2 min,const glm::vec2 max)
    : m_texture(texture)
{
    m_texCoords[0] = {min.x,min.y};
    m_texCoords[1] = {max.x,min.y};
    m_texCoords[2] = {max.x,max.y};
    m_texCoords[3] = {min.x,max.y};
}

const glm::vec2* SubTexture2D::getTexCoords()const
{
    return m_texCoords;
}

const Ref<Texture2D> SubTexture2D::getTexture()const
{
    return m_texture;
}

Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture,const glm::vec2& coords,const glm::vec2& cellSize,const glm::vec2& spriteSize)
{
    const glm::vec2 min =   {(coords.x * cellSize.x)/texture->getWidth(),(coords.y * cellSize.y)/texture->getHeight()};
    const glm::vec2 max =   {((coords.x + spriteSize.x)* cellSize.x)/texture->getWidth(),((coords.y + spriteSize.y) * cellSize.y)/texture->getHeight()};

    return CreateRef<SubTexture2D>(texture,min,max);
}

} // namespace Rengin