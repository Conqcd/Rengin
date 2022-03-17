#include "repch.hpp"
#include "WindowExternalExe.hpp"

namespace Rengin
{
    
WindowExternalExe::WindowExternalExe()
{
}

WindowExternalExe::~WindowExternalExe()
{
    if(hasProcess)
    {
        Terminate();
    }
}

Ref<ExternalExe> ExternalExe::Create()
{
    return CreateRef<WindowExternalExe>();
}

void WindowExternalExe::CreateProcess(const std::string &path)
{
    ZeroMemory(&m_StartUpInfo, sizeof(m_StartUpInfo));
    ZeroMemory(&m_ProcessInfo, sizeof(m_ProcessInfo));
    if(!::CreateProcess("mpiexec.exe","-n 4 femsolver.exe temp/vo.txt",NULL,NULL,false,0,NULL,NULL,&m_StartUpInfo,&m_ProcessInfo))
    {
        RE_CORE_ERROR("Can not open the process");
        return;
    }
    hasProcess = true;
}

void WindowExternalExe::WaitProcess()
{
    if(hasProcess)
    {
        WaitForSingleObject(m_ProcessInfo.hProcess, INFINITE);
    }
}

void WindowExternalExe::Terminate()
{
    TerminateProcess(m_ProcessInfo.hProcess,0);
    hasProcess = false;
}

} // namespace Rengin

