#pragma once
#ifdef RE_PLATFORM_WINDOWS

extern Rengin::Application* Rengin::CreateApplication(); 

int main(int argc,char **argv)
{
    ::Rengin::Log::Init();
    RE_PROFILE_BEGIN_SESSION("StartUp","Rengin-Startup.json");
    auto app = Rengin::CreateApplication();
    RE_PROFILE_END_SESSION();
    RE_PROFILE_BEGIN_SESSION("StartUp","Rengin-Runtime.json");
    app->Run();
    RE_PROFILE_END_SESSION();
    RE_PROFILE_BEGIN_SESSION("StartUp","Rengin-Shutdown.json");
    delete app;
    RE_PROFILE_END_SESSION();
    return 0;
}

#endif