#pragma once

#ifdef YNM_PLATFORM_WINDOWS
	
extern ynm::Application* ynm::CreateApplication();

int main(int argc, char** argv) {

	ynm::Log::Init();
	YNM_CORE_INFO("YUNOMI");
	int a = 69;
	YNM_WARN("CLIENT Var={0}", a);
	
	auto app = ynm::CreateApplication();
	app->Run();
	delete app;
}

#endif