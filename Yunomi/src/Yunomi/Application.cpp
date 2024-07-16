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
			objManager = new ObjectManager(renderer);

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

}

