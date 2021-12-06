#include <rengine.hpp>

class SandBox :public Rengin::Application
{
public:
    SandBox(){}

    ~SandBox(){}

};


Rengin::Application* Rengin::CreateApplication()
{
    return new SandBox();
}