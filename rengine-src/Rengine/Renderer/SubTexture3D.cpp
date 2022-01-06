#include "repch.hpp"
#include "SubTexture3D.hpp"

namespace Rengin
{

SubTexture3D::SubTexture3D(const Ref<Texture3D>& texture,const glm::vec3 min,const glm::vec3 max)
    : m_texture(texture)
{
    m_texCoords[0] = {min.x,min.y,min.z};
    m_texCoords[1] = {max.x,min.y,min.z};
    m_texCoords[2] = {max.x,max.y,min.z};
    m_texCoords[3] = {min.x,max.y,min.z};
    m_texCoords[4] = {min.x,min.y,max.z};
    m_texCoords[5] = {max.x,min.y,max.z};
    m_texCoords[6] = {max.x,max.y,max.z};
    m_texCoords[7] = {min.x,max.y,max.z};
}

const glm::vec3* SubTexture3D::getTexCoords()const
{
    return m_texCoords;
}

const Ref<Texture3D> SubTexture3D::getTexture()const
{
    return m_texture;
}

Ref<SubTexture3D> SubTexture3D::CreateFromCoords(const Ref<Texture3D>& texture,const glm::vec3& coords,const glm::vec3& cellSize,const glm::vec3& spriteSize)
{
    const glm::vec3 min =   {(coords.x * cellSize.x)/texture->getWidth(),(coords.y * cellSize.y)/texture->getHeight(),(coords.z * cellSize.z)/texture->getDepth()};
    const glm::vec3 max =   {((coords.x + spriteSize.x)* cellSize.x)/texture->getWidth(),((coords.y + spriteSize.y) * cellSize.y)/texture->getHeight(),((coords.z + spriteSize.z) * cellSize.z)/texture->getDepth()};

    return CreateRef<SubTexture3D>(texture,min,max);
}

} // namespace Rengin