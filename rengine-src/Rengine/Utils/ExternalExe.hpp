#pragma once
#include "Rengine/Core/core.hpp"
#include <string>

namespace Rengin
{

class RE_API ExternalExe
{
public:
    virtual ~ExternalExe() = default;
    static Ref<ExternalExe> Create();
    virtual void CreateProcess(const std::string& exePath,const std::string& Args) = 0;
    virtual void WaitProcess() = 0;
    virtual void Terminate() = 0;
};

} // namespace Rengin
