#pragma once
#include <filesystem>

namespace Rengin
{
class ContentBrowserPanel
{
private:
    std::filesystem::path m_CurrentDirectory;
public:
    ContentBrowserPanel(/* args */);
    ~ContentBrowserPanel() = default;

    void OnImGuiRender();
};


} // namespace Rengin
