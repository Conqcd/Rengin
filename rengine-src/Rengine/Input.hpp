#pragma once
#include "../repch.hpp"
#include "core.hpp"

namespace Rengin
{
class RE_API Input
{
private:
    static Input* s_instance;
public:
    inline static bool isKeyPressed(int key_code) {return s_instance->isKeyPressedImpl(key_code);}
    
    inline static bool isMouseButtonPressed(int button) {return s_instance->isMouseButtonPressedImpl(button);}
    inline static std::pair<float,float> getMousePosition() {return s_instance->MousePosImpl();}
    inline static float getMouseX() {return s_instance->MouseXImpl();}
    inline static float getMouseY() {return s_instance->MouseYImpl();}
protected:
    virtual bool isKeyPressedImpl(int key_code) = 0;

    virtual bool isMouseButtonPressedImpl(int button) = 0;
    virtual std::pair<float,float> MousePosImpl() = 0;
    virtual float MouseXImpl() = 0;
    virtual float MouseYImpl() = 0;
};

} // namespace Rengin