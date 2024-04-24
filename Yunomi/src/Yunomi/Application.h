#pragma once
#include "Core.h"
#include "Render/Window.h"
#include "Render/Instance.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace ynm {

	class YNM_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		Window* m_Window;
		Instance* m_Instance;
		bool m_Running = true;

	};

	//To be defined in client
	ynm::Application* CreateApplication();
}

