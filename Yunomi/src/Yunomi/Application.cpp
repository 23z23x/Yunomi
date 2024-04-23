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
		m_Window = std::unique_ptr<Window>(Window::Create());

		glm::mat4 matrix;
		glm::vec4 vec;
		auto test = matrix * vec;

	}

	Application::~Application() 
	{

	}

	void Application::Run() 
	{
		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		VulkanInstance vkInstance = VulkanInstance(validationLayers);
		while (m_Running)
		{
			m_Window->OnUpdate();

		}
	}

}

