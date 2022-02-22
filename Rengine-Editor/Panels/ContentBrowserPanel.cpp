#include <repch.hpp>
#include "ContentBrowserPanel.hpp"
#include <imgui.h>
#include <imgui_internal.h>

namespace Rengin {

constexpr char* s_AssetsDiretory = "assets";
static const std::filesystem::path s_AssetsPath = "assets";

ContentBrowserPanel::ContentBrowserPanel()
    :   m_CurrentDirectory(s_AssetsDiretory)
{
}

void ContentBrowserPanel::OnImGuiRender()
{
    ImGui::Begin("Content Browser");

    if(m_CurrentDirectory != std::filesystem::path(s_AssetsDiretory))
    {
        if(ImGui::Button("<-"))
        {
            m_CurrentDirectory = m_CurrentDirectory.parent_path();
        }
    }

    for (auto &directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
    {
        const auto& path = directoryEntry.path();
        auto relativePath = std::filesystem::relative(path,s_AssetsPath);
        std::string pathString = relativePath.filename().string();
        // auto p0 = p.path().filename().string();
        // auto p1 = p.path().stem().string();
        // ImGui::Text("Filename %s",p0.c_str());
        // ImGui::Text("Stem %s",p1.c_str());
        if(directoryEntry.is_directory())
        {
            if(ImGui::Button(pathString.c_str()))
            {
                m_CurrentDirectory /= path.filename();
            }
        }else
        {
          if (ImGui::Button(pathString.c_str())) {
            m_CurrentDirectory /= path.filename();
          }
        }
    }
    

    ImGui::End();
}

} // namespace Rengin
