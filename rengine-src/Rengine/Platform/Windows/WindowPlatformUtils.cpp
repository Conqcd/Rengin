#include "repch.hpp"
#include "Rengine/Core/application.hpp"
#include "Rengine/Utils/PlatformUtils.hpp"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Rengin
{
std::string FileDialogs::OpenFile(const char* filter)
{
    OPENFILENAMEA ofn;
    CHAR szFile[260] = {0};

    ZeroMemory(&ofn,sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::getApplication().getWindow().getNativeWindow());
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn) == TRUE)
    {
        return ofn.lpstrFile;
    }
    
    return {};
}

std::string FileDialogs::SaveFile(const char* filter)
{
    OPENFILENAMEA ofn;
    CHAR szFile[260] = {0};

    ZeroMemory(&ofn,sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::getApplication().getWindow().getNativeWindow());
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetSaveFileNameA(&ofn) == TRUE)
    {
        return ofn.lpstrFile;
    }
    

    return {};
}

   
} // namespace Rengin
