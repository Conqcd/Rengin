#include"Event.hpp"

namespace Rengin
{
class RE_API MouseMovedEvent :public KeyEvent
{
private:
    float m_x,m_y;
public:
    MouseMovedEvent(float _x,float _y):m_x(_x),m_y(_y){}
    inline float getX()const {return m_x;}
    inline float getY()const {return m_y;}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss<<"MouseMovedEvent x:"<< m_x <<" ,y:" << m_y ;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATAGORY(EventCategoryInput|EventCategoryMouse)
};

class RE_API MouseScrolledEvent :public KeyEvent
{
private:
    float m_Xoffset,m_Yoffset;
public:
    MouseScrolledEvent(float _x,float _y):m_Xoffset(_x),m_Yoffset(_y){}
    inline float getXoffset()const {return m_Xoffset;}
    inline float getYoffset()const {return m_Yoffset;}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss<<"MouseScrolledEvent xoffset:"<< m_Xoffset <<" ,y:" << m_Yoffset;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATAGORY(EventCategoryInput|EventCategoryMouse)
};

class RE_API MouseButtonEvent :public KeyEvent
{
private:
public:
    inline int getButton()const {return m_button;}
    EVENT_CLASS_CATAGORY(EventCategoryInput|EventCategoryMouse)
protected:
    explicit MouseButtonEvent(int m_b):m_button(m_b){}
    int m_button;
};

class RE_API MouseButtonPressEvent :public MouseButtonEvent
{
public:
    inline int getButton()const {return m_button;}
    EVENT_CLASS_TYPE(MouseButtonPress)

    std::string ToString() const override
    {
        std::stringstream ss;
        ss<<"MouseButtonPressEvent "<< m_button ;
        return ss.str();
    }

    explicit MouseButtonPressEvent(int m_b):MouseButtonEvent(m_b){}
};

class RE_API MouseButtonReleaseEvent :public MouseButtonEvent
{
public:
    inline int getButton()const {return m_button;}
    EVENT_CLASS_TYPE(MouseButtonRelease)

    std::string ToString() const override
    {
        std::stringstream ss;
        ss<<"MouseButtonReleaseEvent "<< m_button ;
        return ss.str();
    }

    explicit MouseButtonReleaseEvent(int m_b):MouseButtonEvent(m_b){}
};
}