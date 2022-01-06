#pragma once
#include <glm/glm.hpp>
#include "Texture.hpp"


namespace Rengin
{
class SubTexture3D 
{
private:
    Ref<Texture3D> m_texture;
    glm::vec3 m_texCoords[8];
public:
    SubTexture3D(const Ref<Texture3D>& texture,const glm::vec3 min,const glm::vec3 max);
    const glm::vec3* getTexCoords()const;
    const Ref<Texture3D> getTexture()const;

    static Ref<SubTexture3D> CreateFromCoords(const Ref<Texture3D>& texture,const glm::vec3& coords,const glm::vec3& cellSize,const glm::vec3& spriteSize = {1.0f ,1.0f,1.0f});
};

} // namespace Rengin