#include "pch.h"

//Also later, look into VulkanMemoryAllocator library by GPUOpen initiative
//And RAII

#include "Application.h"
#include "Log.h"

#include <iostream>

namespace ynm {

	//Defines
	//std::bind is like passing a function, except you can use placeholders to automatically fill in certain parameters
	#define BIND_EVENT_FN(X) std::bind(&X, this, std::placeholders::_1)

	
	Application::Application() 
	{
		Shader* vertShader = Shader::Create("C:/repos/Yunomi/Yunomi/src/Yunomi/Shaders/shader.vert", ShaderType::VRTX);
		Shader* fragShader = Shader::Create("C:/repos/Yunomi/Yunomi/src/Yunomi/Shaders/shader.frag", ShaderType::FRAG);

		m_Window = Window::Create();
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_Instance = Instance::Create(m_Window, vertShader, fragShader);


	}

	Application::~Application() 
	{

	}

	void Application::Run() 
	{
		//Texture* text = Texture::Create(m_Instance, "C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/viking_room.png");
		//Mesh mesh = Mesh("C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/viking_room.obj");
		Texture* text = Texture::Create(m_Instance, "C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/texture.jpg");
		Quad mesh = Quad(-0.5f, 0.5f, 0.5f, -0.5f, 0.0f);

		VertexBuffer* vertBuffer = VertexBuffer::Create(m_Instance, mesh.getVertices());
		IndexBuffer* indbuffer = IndexBuffer::Create(m_Instance, mesh.getIndices());
		UniformBuffer* unifBuffer = UniformBuffer::Create(m_Instance);

		m_Instance->AddDescriptors(unifBuffer, text);

		while (mainLoop)
		{
			m_Window->OnUpdate();
			m_Instance->StartDraw(vertBuffer, indbuffer);
			m_Instance->UpdateUniform(unifBuffer);
			m_Instance->EndDraw();

		}
		m_Window->~Window();
	}

	void Application::OnEvent(Event& e)
	{
		//YNM_CORE_INFO("{0}", e);

		switch (e.GetEventType())
		{
		case EventType::WinResize:
			m_Instance->FrameResize();
			break;
		case EventType::WinClose:
			mainLoop = false;
		}
	}

}

