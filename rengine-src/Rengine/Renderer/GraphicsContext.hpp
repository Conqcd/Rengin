#pragma once
#include "Rengine/Core/core.hpp"
namespace Rengin
{

class GraphicsContext
{
private:
    /* data */
public:
    virtual void Init() = 0;
    virtual void SwapBuffer() = 0;
};


} // namespace Rengin
