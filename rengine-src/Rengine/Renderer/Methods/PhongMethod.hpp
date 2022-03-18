#pragma once
#include "Rengine/Core/core.hpp"
#include "BaseMethod.hpp"
#include "Rengine/Scene/ObjManager.hpp"

namespace Rengin
{
class PhongMethod : public RenderMethod
{
private:

public:
    PhongMethod();
    virtual ~PhongMethod();

    virtual void Render() override; 
};
} // namespace Rengin
