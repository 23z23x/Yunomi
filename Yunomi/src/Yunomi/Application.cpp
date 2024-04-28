#include "pch.h"

#include "Application.h"
#include "Log.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

namespace ynm {
	
	Application::Application() 
	{
		Shader* vertShader = Shader::Create("C:/repos/Yunomi/Yunomi/src/Yunomi/Shaders/shader.vert", ShaderType::Vertex);
		Shader* fragShader = Shader::Create("C:/repos/Yunomi/Yunomi/src/Yunomi/Shaders/shader.frag", ShaderType::Fragment);

		m_Window = Window::Create();
		m_Instance = Instance::Create(m_Window);

		glm::mat4 matrix;
		glm::vec4 vec;
		auto test = matrix * vec;

	}

	Application::~Application() 
	{

	}

	void Application::Run() 
	{
		while (m_Running)
		{
			m_Window->OnUpdate();

		}
	}

}

