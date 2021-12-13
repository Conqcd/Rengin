#include "repch.hpp"
#include "WindowInput.hpp"

#include <GLFW/glfw3.h>
#include "Rengine/application.hpp"

namespace Rengin
{


Input* Input::s_instance = new WindowInput();

WindowInput::WindowInput(/* args */)
{

}

WindowInput::~WindowInput()
{

}

bool WindowInput::isKeyPressedImpl(int key_code) 
{
    auto window = static_cast<GLFWwindow*> (Application::getApplication().getWindow().getNativeWindow());

    auto status = glfwGetKey(window,key_code);
    return status == GLFW_PRESS || status == GLFW_REPEAT;
}


bool WindowInput::isMouseButtonPressedImpl(int button)
{
    auto window = static_cast<GLFWwindow*> (Application::getApplication().getWindow().getNativeWindow());

    auto status = glfwGetMouseButton(window,button);
    return status == GLFW_PRESS;
}

std::pair<float,float> WindowInput::MousePosImpl()
{
    auto window = static_cast<GLFWwindow*> (Application::getApplication().getWindow().getNativeWindow());
    double xPos,yPos;
    glfwGetCursorPos(window,&xPos,&yPos);
    return {static_cast<float>(xPos),static_cast<float>(yPos)};
}

float WindowInput::MouseXImpl()
{
    auto [xPos,yPos] = MousePosImpl();
    return xPos;
}

float WindowInput::MouseYImpl()
{
    auto [xPos,yPos] = MousePosImpl();
    return yPos;
}

} // namespace Rengin
