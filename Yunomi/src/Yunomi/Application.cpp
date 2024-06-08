#include "pch.h"

//Also later, look into VulkanMemoryAllocator library by GPUOpen initiative
//And RAII

#include "Application.h"

namespace ynm {

	//Defines
	//std::bind is like passing a function, except you can use placeholders to automatically fill in certain parameters
	#define BIND_EVENT_FN(X) std::bind(&X, this, std::placeholders::_1)
	
	Application::Application() 
	{
		Shader* vertShader = Shader::Create("C:/repos/Yunomi/Yunomi/src/Yunomi/Shaders/shader.vert", ShaderType::VRTX);
		Shader* fragShader = Shader::Create("C:/repos/Yunomi/Yunomi/src/Yunomi/Shaders/shader.frag", ShaderType::FRAG);

		window = Window::Create();
		window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		renderer = new Renderer(window, vertShader, fragShader);


	}

	Application::~Application() 
	{

	}

	void Application::Run() 
	{
		//Test Game Objects
		uint32_t vikingID = renderer->CreateTexture("C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/viking_room.png");
		uint32_t shizID = renderer->CreateTexture("C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/texture.jpg");
		uint32_t fishID = renderer->CreateTexture("C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/fish.png");

		Mesh quad = renderer->CreateQuad(-0.5f, 0.5f, 0.5f, -0.5f, 0.0f);
		Mesh mesh = renderer->CreateMesh("C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/viking_room.obj");

		InstanceData shizData;
		shizData.modelMatrix = glm::translate(shizData.modelMatrix, { 2.0f, 1.0f, 0.0f });
		GameObject shiz = GameObject(0, "Shiz", shizData, shizID, &quad);
		InstanceData fishData;
		fishData.modelMatrix = glm::rotate(fishData.modelMatrix, glm::radians(30.0f), { 1.0f, 0.0f, 0.0f });
		GameObject fish = GameObject(1, "Fish", fishData, fishID, &quad);
		InstanceData vikingData;
		vikingData.modelMatrix = glm::scale(vikingData.modelMatrix, { 0.5f, 0.5f, 0.5f });
		GameObject viking = GameObject(2, "Viking", vikingData, vikingID, &mesh);

		std::vector<GameObject> objs;
		objs.push_back(shiz);
		objs.push_back(fish);
		objs.push_back(viking);
		
		renderer->LoadObjects(objs);

		renderer->AddDescriptors();

		while (mainLoop)
		{
			window->OnUpdate();
			renderer->StartDraw();
			renderer->UpdateUniform();
			renderer->EndDraw();

		}
		window->~Window();
	}

	void Application::OnEvent(Event& e)
	{
		//YNM_CORE_INFO("{0}", e);

		switch (e.GetEventType())
		{
		case EventType::WinResize:
			renderer->FrameResize();
			break;
		case EventType::WinClose:
			mainLoop = false;
		}
	}

}

