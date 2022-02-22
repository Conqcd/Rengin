#pragma once
#include "Rengine/Renderer/Texture.hpp"
#include <filesystem>

namespace Rengin
{
class ContentBrowserPanel
{
private:
    std::filesystem::path m_CurrentDirectory;
    Ref<Texture2D> m_DirectoryIcon;
    Ref<Texture2D> m_FileIcon;
public:
    ContentBrowserPanel(/* args */);
    ~ContentBrowserPanel() = default;

    void OnImGuiRender();
};


} // namespace Rengin
