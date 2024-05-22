#include "pch.h"

//Also later, look into VulkanMemoryAllocator library by GPUOpen initiative
//And RAII

#include "Application.h"
#include "Log.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

namespace ynm {

	//Defines
	//std::bind is like passing a function, except you can use placeholders to automatically fill in certain parameters
	#define BIND_EVENT_FN(X) std::bind(&X, this, std::placeholders::_1)

	//Position, Color, Texture mapping for corner
	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

		{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 6, 7, 4
	};

	
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
		VertexBuffer* vertBuffer = VertexBuffer::Create(m_Instance, vertices);
		IndexBuffer* indbuffer = IndexBuffer::Create(m_Instance, indices);
		UniformBuffer* unifBuffer = UniformBuffer::Create(m_Instance);

		Texture* text = Texture::Create(m_Instance, "C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/texture.jpg");

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

