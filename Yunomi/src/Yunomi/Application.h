#pragma once
#include "Core.h"
#include "Render/Window.h"
#include "Render/Instance.h"
#include "Render/Shader.h"
#include "Render/Buffer.h"
#include "Render/Texture.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace ynm {

	class YNM_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		bool moved;
		Window* m_Window;
		Instance* m_Instance;

	};

	//To be defined in client
	ynm::Application* CreateApplication();
}

