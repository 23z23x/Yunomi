#include "pch.h"

#include "Application.h"
#include "Log.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

namespace ynm {

	//Position, Color, Texture mapping for corner
	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
	};

	const std::vector<uint32_t> indices = {
	0, 1, 2, 2, 3, 0
	};

	
	Application::Application() 
	{
		Shader* vertShader = Shader::Create("C:/repos/Yunomi/Yunomi/src/Yunomi/Shaders/shader.vert", ShaderType::VRTX);
		Shader* fragShader = Shader::Create("C:/repos/Yunomi/Yunomi/src/Yunomi/Shaders/shader.frag", ShaderType::FRAG);

		m_Window = Window::Create();
		m_Instance = Instance::Create(m_Window, vertShader, fragShader);

		VertexBuffer* vertBuffer = VertexBuffer::Create(m_Instance, vertices);
		IndexBuffer* indbuffer = IndexBuffer::Create(m_Instance, indices);
		UniformBuffer* unifBuffer = UniformBuffer::Create(m_Instance);

		vertBuffer->~VertexBuffer();
		indbuffer->~IndexBuffer();
		unifBuffer->~UniformBuffer();

		Texture* text = Texture::Create(m_Instance, "C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/texture.jpg");

		text->~Texture();

		glm::mat4 matrix;
		glm::vec4 vec;
		auto test = matrix * vec;

		m_Instance->~Instance();

	}

	Application::~Application() 
	{

	}

	void Application::Run() 
	{
		while (!m_Window->ShouldClose())
		{
			m_Window->OnUpdate();

		}
		m_Window->~Window();
	}

}

