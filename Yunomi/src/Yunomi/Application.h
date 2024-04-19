#pragma once
#include "Core.h"

namespace ynm {

	class YNM_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//To be defined in client
	ynm::Application* CreateApplication();
}

