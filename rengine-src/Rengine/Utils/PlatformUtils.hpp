#pragma once
#include <string>

namespace Rengin
{
class FileDialogs
{
private:
    /* data */
public:
    static std::string OpenFile(const char* filter);
    static std::string SaveFile(const char* filter);
};

} // namespace Rengin
