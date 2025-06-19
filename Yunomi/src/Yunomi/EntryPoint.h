#pragma once

#ifdef YNM_PLATFORM_WINDOWS

extern ynm::Application* ynm::CreateApplication();

int main(int argc, char** argv) {
    ynm::Log::Init();
    YNM_CORE_INFO("YUNOMI");

    auto app = ynm::CreateApplication();
    app->Run();
    delete app;
    return 0;
}

#elif defined(YNM_PLATFORM_LINUX)

extern ynm::Application* ynm::CreateApplication();

int main(int argc, char** argv) {
    ynm::Log::Init();
    YNM_CORE_INFO("YUNOMI");

    auto app = ynm::CreateApplication();
    app->Run();
    delete app;
    return 0;
}

#else
    #error "Unsupported platform"
#endif
