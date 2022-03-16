#pragma once
#include "Rengine/Utils/ExternalExe.hpp"
#include "Windows.h"

namespace Rengin
{
class RE_API WindowExternalExe : public ExternalExe
{
private:
    bool hasProcess = false;
    STARTUPINFO m_StartUpInfo;  
    PROCESS_INFORMATION m_ProcessInfo;  
public:
    WindowExternalExe();
    virtual ~WindowExternalExe();

    virtual void CreateProcess(const std::string& path) override;
    virtual void WaitProcess() override;
    virtual void Terminate() override;
};
} // namespace Rengin
