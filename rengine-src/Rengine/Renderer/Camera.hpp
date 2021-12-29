#pragma once

#include <glm/glm.hpp>

namespace Rengin
{
class Camera
{
protected:
    glm::mat4 m_Projection = glm::mat4(1.0f);
public:
    Camera() = default;
    Camera(const glm::mat4& projection)
        : m_Projection(projection) {}
    virtual ~Camera() = default;
    const glm::mat4& getProjection() const {return m_Projection;}
};

    
} // namespace Rengin
