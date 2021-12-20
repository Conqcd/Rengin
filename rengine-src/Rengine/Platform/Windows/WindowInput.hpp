#pragma once
#include "Rengine/Core/Input.hpp"
#include "Rengine/Core/core.hpp"

namespace Rengin
{
class RE_API WindowInput :public Input
{
private:
    /* data */
public:
    WindowInput(/* args */);
    ~WindowInput();
protected:
    virtual bool isKeyPressedImpl(int key_code) override;
    
    virtual bool isMouseButtonPressedImpl(int button) override;
    virtual std::pair<float,float> MousePosImpl() override;
    virtual float MouseXImpl() override;
    virtual float MouseYImpl() override;
};



} // namespace Rengin
