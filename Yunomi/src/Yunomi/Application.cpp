#include "pch.h"

//Also later, look into VulkanMemoryAllocator library by GPUOpen initiative
//And RAII

#include "Application.h"
#include "Event/InputEvent.h"

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

		InstanceData shizData;
		shizData.ID = 0;
		shizData.textureID = shizID;
		std::cout << shizID << std::endl;

		InstanceData fishData;
		fishData.ID = 1;
		fishData.textureID = fishID;
		std::cout << fishID << std::endl;

		InstanceData vikingData;
		vikingData.ID = 2;
		vikingData.textureID = vikingID;
		std::cout << vikingID << std::endl;



		GameObject shiz = GameObject("Shiz", shizData, &quad);
		GameObject fish = GameObject("Fish", fishData, &quad);
		GameObject viking = GameObject("Viking", vikingData, &mesh);
		GameObject viking2 = GameObject("Viking2", vikingData, &mesh);
		GameObject viking3 = GameObject("Vking3", vikingData, &mesh);

		shiz.Translate(glm::vec3(2.0f, 1.0f, 1.5f));
		shiz.RotateX(20.0);
		shiz.RotateY(34.0);

		fish.Translate(glm::vec3(-1.0f, -1.0f, 0.0f));

		viking.Scale(glm::vec3(0.5f, 0.5f, 0.5f));

		viking2.Translate(glm::vec3(3.0f, 3.0f, 0.0f));

		viking3.Translate(glm::vec3(2.0f, -2.5f, 1.5f));
		//viking3.RotateX(75);
		//viking3.RotateZ(90);

		

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

		KeyDownEvent* kd = nullptr;
		MouseMovedEvent* mm = nullptr;


		switch (e.GetEventType())
		{
		case EventType::WinClose:
			mainLoop = false;
			break;
		case EventType::KeyDown:
			kd = (KeyDownEvent*)&e;
			renderer->ChangePosition(kd->GetKeyCode());
			break;
		case EventType::MouseMove:
			mm = (MouseMovedEvent*)&e;
			renderer->ChangeDirection(mm->GetX(), mm->GetY());
			break;
		}
	}

}

