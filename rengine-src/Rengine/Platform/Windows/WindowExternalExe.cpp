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

void WindowExternalExe::CreateProcess(const std::string &exePath,const std::string& Args)
{
    ZeroMemory(&m_StartUpInfo, sizeof(m_StartUpInfo));
    ZeroMemory(&m_ProcessInfo, sizeof(m_ProcessInfo));
    m_StartUpInfo.cb = sizeof(m_StartUpInfo);
    m_StartUpInfo.wShowWindow = SW_SHOW;
    m_StartUpInfo.dwFlags = STARTF_USESHOWWINDOW;
    m_StartUpInfo.lpTitle ="femSolver";
    if(!::CreateProcess((LPCSTR)exePath.c_str(),(LPSTR)Args.c_str(),NULL,NULL,false,0,NULL,NULL,&m_StartUpInfo,&m_ProcessInfo))
    {
        RE_CORE_ERROR("Can not open the process");
        return;
    }
    // HWND console_name = FindWindow(NULL,"my_process_console");
    // if (console_name) {
    //   ShowWindow(console_name, SW_SHOW);
    // }
    hasProcess = true;
}

void WindowExternalExe::WaitProcess()
{
    if(hasProcess)
        WaitForSingleObject(m_ProcessInfo.hProcess, INFINITE);
    hasProcess = false;
}

void WindowExternalExe::Terminate()
{
    if(hasProcess)
        TerminateProcess(m_ProcessInfo.hProcess, 0);
    hasProcess = false;
}

} // namespace Rengin

