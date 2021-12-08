
#ifdef RE_WINDOWS

extern Rengin::Application* Rengin::CreateApplication(); 

int main(int argc,char **argv)
{
    ::Rengin::Log::Init();
    RE_CORE_ERROR("hadosanc");
    RE_ERROR("hihicore");
    auto app = Rengin::CreateApplication();
    app->Run();
    delete app;
    return 0;
}

#endif