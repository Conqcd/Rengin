#pragma once

namespace Rengin
{
class TimeStep
{
private:
    float m_time;
public:
    TimeStep(float time = 0.0f)
        :m_time(time)
    {
    }
    
    operator float()const {return m_time;}

    float getSeconds()const {return m_time;}
    float getMilliSeconds()const {return m_time * 1000.0f;}
};

} // namespace Rengin
