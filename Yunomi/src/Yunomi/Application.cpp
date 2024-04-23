#include "pch.h"

#include "Application.h"
#include "Log.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

namespace ynm {
	
	Application::Application() 
	{
		m_Window = std::unique_ptr<Window>(Window::Create());

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::cout << extensionCount << " extensions supported\n";

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

