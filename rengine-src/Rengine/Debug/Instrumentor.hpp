#pragma once
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <thread>

namespace Rengin
{

struct ProfileResult
{
    std::string m_name;
    long long m_start,m_end;
    uint32_t m_ThreadId;
};

struct InstrumentorSession
{
    std::string m_name;
    explicit InstrumentorSession(const std::string& name):m_name(name){}
};

class Instrumentor
{
private:
    InstrumentorSession* m_CurrentSession;
    int m_ProfileCount;
    std::ofstream m_OutputStream;
public:
    Instrumentor()
        : m_CurrentSession(nullptr),m_ProfileCount(0)
    {   
    }

    void BeginSession(const std::string& name,const std::string& filePath = "result.json")
    {
        m_OutputStream.open(filePath);
        WriterHeader();
        m_CurrentSession = new InstrumentorSession(name);
    }

    void EndSession()
    {
        WriteFooter();
        m_OutputStream.close();
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
        m_ProfileCount = 0;
    }

    void WriteProfile(const ProfileResult& result)
    {
        if(m_ProfileCount++ > 0)
            m_OutputStream << ",";

        std::string name = result.m_name;
        std::replace(name.begin(),name.end(),'"','\'');

        m_OutputStream << "{";
        m_OutputStream << "\"cat\":\"function\",";
        m_OutputStream << "\"dur\":" << (result.m_end - result.m_start) << ',';
        m_OutputStream << "\"name\":\"" << name << "\",";
        m_OutputStream << "\"ph\":\"X\",";
        m_OutputStream << "\"pid\":0,";
        m_OutputStream << "\"tid\":" << result.m_ThreadId <<",";
        m_OutputStream << "\"ts\":" << result.m_start;
        m_OutputStream << "}";
        m_OutputStream.flush();
    }

    void WriterHeader()
    {
        m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
        m_OutputStream.flush();
    }

    void WriteFooter()
    {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

    static Instrumentor& Get()
    {
        static Instrumentor instrumentor;
        return instrumentor;
    }
};

class InstrumentationTimer
{
private:
    const char* m_name;
    bool m_stop;
    std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
public:
    InstrumentationTimer(const char* name)
        : m_name(name), m_stop(false)
    {
        m_StartTimePoint = std::chrono::high_resolution_clock::now();
    }
    ~InstrumentationTimer()
    {
        if(!m_stop)
            Stop();
    } 
    void Stop()
    {
        auto m_EndTimePoint = std::chrono::high_resolution_clock::now();
        
        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(m_EndTimePoint).time_since_epoch().count();

        uint32_t threadId = static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));

        Instrumentor::Get().WriteProfile({m_name,start,end,threadId});
        m_stop = true;
    }
};

} // namespace Rengin
#define RE_PROFILE 1
#if RE_PROFILE
    #define RE_PROFILE_BEGIN_SESSION(name,filepath) ::Rengin::Instrumentor::Get().BeginSession(name,filepath)
    #define RE_PROFILE_END_SESSION() ::Rengin::Instrumentor::Get().EndSession()
    #define RE_PROFILE_SCOPE(name) ::Rengin::InstrumentationTimer timer##__LINE__(name)
    #define RE_PROFILE_FUNCTION() RE_PROFILE_SCOPE(__FUNCSIG__)
#else
    #define RE_PROFILE_BEGIN_SESSION(name)
    #define RE_PROFILE_END_SESSION()
    #define RE_PROFILE_SCOPE(name)
    #define RE_PROFILE_FUNCTION()
#endif