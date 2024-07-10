#pragma once
#include "Core.h"
#include "Render/Window.h"
#include "Render/Renderer.h"
#include "Render/Geometry.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace ynm {

	class YNM_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		virtual void OnEvent(Event& e) = 0;

		Window* window;
		Renderer* renderer;
		bool mainLoop = true;
	private:
		bool moved;

	};

	//To be defined in client
	ynm::Application* CreateApplication();
}

