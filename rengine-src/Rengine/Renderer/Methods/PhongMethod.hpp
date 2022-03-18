#pragma once
#include "Rengine/Core/core.hpp"
#include "BaseMethod.hpp"

namespace Rengin
{
class PhongMethod : public RenderMethod
{
private:
public:
    PhongMethod(/* args */);
    virtual ~PhongMethod();

    virtual void Render() override; 
};
} // namespace Rengin
