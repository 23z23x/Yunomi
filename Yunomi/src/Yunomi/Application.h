#pragma once
#include "Core.h"
#include "Window.h"
#include "Platform/Vulkan/VulkanInstance.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"

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

