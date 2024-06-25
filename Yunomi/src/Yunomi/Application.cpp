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
		try
		{
			Shader* vertShader = Shader::Create("C:/repos/Yunomi/Yunomi/src/Yunomi/Shaders/shader.vert", ShaderType::VRTX);
			Shader* fragShader = Shader::Create("C:/repos/Yunomi/Yunomi/src/Yunomi/Shaders/shader.frag", ShaderType::FRAG);

			window = Window::Create();
			window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
			renderer = new Renderer(window);

			PipelineProps pipelineProps;
			pipelineProps.Vertex = vertShader;
			pipelineProps.Fragment = fragShader;
			pipelineProps.msaaLevel = MSAA_4;

			uint32_t pipelineID = renderer->CreatePipeline(pipelineProps);
			renderer->SetPipeline(pipelineID);
		}
		catch (std::exception& e)
		{
			YNM_CORE_ERROR("Fatal error occured in Application initialization.");
			exit(0);
		}

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

		InstanceData defaultData;
		GameObject shiz = GameObject(0, "Shiz", defaultData, shizID, &quad);
		GameObject fish = GameObject(1, "Fish", defaultData, fishID, &quad);
		GameObject viking = GameObject(2, "Viking", defaultData, vikingID, &mesh);
		GameObject viking2 = GameObject(3, "Viking2", defaultData, vikingID, &mesh);
		GameObject viking3 = GameObject(4, "Vking3", defaultData, vikingID, &mesh);

		shiz.Translate(glm::vec3(2.0f, 1.0f, 1.5f));
		shiz.RotateX(20.0);
		shiz.RotateY(34.0);

		fish.Translate(glm::vec3(-1.0f, -1.0f, 0.0f));

		viking.Scale(glm::vec3(0.5f, 0.5f, 0.5f));

		viking2.Translate(glm::vec3(3.0f, 3.0f, 0.0f));

		viking3.Translate(glm::vec3(3.5f, -2.0f, 0.0f));
		viking3.RotateX(75);
		viking3.RotateZ(90);

		

		std::vector<GameObject> objs;
		objs.push_back(shiz);
		objs.push_back(fish);
		objs.push_back(viking);
		objs.push_back(viking2);
		objs.push_back(viking3);
		
		renderer->LoadObjects(objs);

		renderer->AddDescriptors();

		while (mainLoop)
		{
			try
			{
				window->OnUpdate();
				renderer->StartDraw();
				renderer->UpdateUniform();
				renderer->EndDraw();
			}
			catch (std::exception& e)
			{
				YNM_CORE_ERROR("Fatal error occured in main loop.");
				e.what();
				exit(0);
			}
		}
		window->~Window();
	}

	void Application::OnEvent(Event& e)
	{
		//YNM_CORE_INFO("{0}", e);

		switch (e.GetEventType())
		{
		case EventType::WinClose:
			mainLoop = false;
			break;
		}
	}

}

