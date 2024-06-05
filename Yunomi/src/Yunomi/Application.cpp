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
		Texture* text = Texture::Create(m_Instance, "C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/viking_room.png");
		Mesh mesh = Mesh("C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/viking_room.obj");
		Object3D VikingRoom = Object3D(0, "Viking Room", { 0,0,0 }, text, &mesh);
		//Texture* text = Texture::Create(m_Instance, "C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/fish.png");
		//Quad mesh = Quad(-0.5f, 0.5f, 0.5f, -0.5f, 0.0f);

		//Buffer* vertBuffer = Buffer::Create(m_Instance);
		Buffer* indbuffer = Buffer::Create(m_Instance, BufferType::INDEX);
		indbuffer->CreateChunk(mesh.getIndices().size(), 0, (void*)mesh.getIndices().data());
		Buffer* vertBuffer = Buffer::Create(m_Instance, BufferType::VERTEX);
		vertBuffer->CreateChunk(mesh.getVertices().size(), 0, (void*)mesh.getVertices().data());

		//std::cout << mesh.getVertices().data() << std::endl;

		//std::cout << mesh.getIndices().data() << std::endl;

		//std::cout << vertBuffer << " : " << indbuffer << std::endl;

		UniformBuffer* unifBuffer = UniformBuffer::Create(m_Instance);

		m_Instance->AddDescriptors(unifBuffer, text);

		while (mainLoop)
		{
			m_Window->OnUpdate();
			m_Instance->StartDraw(vertBuffer, indbuffer, nullptr);
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

