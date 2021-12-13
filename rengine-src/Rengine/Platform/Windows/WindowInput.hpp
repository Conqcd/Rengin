#pragma once
#include "Rengine/Input.hpp"
#include "Rengine/core.hpp"

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
