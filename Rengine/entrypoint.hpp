
#ifdef RE_WINDOWS

extern Rengin::Application* Rengin::CreateApplication(); 

int main(int argc,char **argv)
{
    auto app = Rengin::CreateApplication();
    app->Run();
    delete app;
    return 0;
}

#endif