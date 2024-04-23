#pragma once

#ifdef YNM_PLATFORM_WINDOWS
	
extern ynm::Application* ynm::CreateApplication();

int main(int argc, char** argv) {

	ynm::Log::Init();
	YNM_CORE_INFO("YUNOMI");
	
	auto app = ynm::CreateApplication();
	app->Run();
	delete app;
}

#endif