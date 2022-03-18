#include "repch.hpp"
#include "PhongMethod.hpp"

namespace Rengin
{
 
PhongMethod::PhongMethod(/* args */)
{
}

PhongMethod::~PhongMethod()
{
}

void PhongMethod::Render()
{

}

template <typename... Args>
void PhongMethod::AddResource(const Args&... resource) {
    static_assert(false);
}

template <> 
void PhongMethod::AddResource<Ref<Shader>>(const Ref<Shader>& base) {
    m_BaseShader = base;
}
} // namespace Rengin
