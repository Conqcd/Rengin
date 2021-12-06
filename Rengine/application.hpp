#pragma once
#include "core.hpp"

namespace Rengin
{

class RE_API Application
{
public:
    Application()
    {

    }
    virtual ~Application()
    {
        
    }

    void Run();
};

Application* CreateApplication();
}
