#pragma once
#include "Core.h"
#include "Window.h"

namespace ynm {

	class YNM_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

	};

	//To be defined in client
	ynm::Application* CreateApplication();
}

