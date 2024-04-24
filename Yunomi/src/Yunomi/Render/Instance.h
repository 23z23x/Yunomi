#pragma once
#include "pch.h"
#include "Yunomi/Core.h"
#include "Window.h"

//Vritual interface class

namespace ynm
{
	struct InstanceProps
	{
		//Platform specific arguments
		std::vector<const char*> vk_ValidationLayers;
		std::vector<const char*> vk_DeviceExtensions;

		InstanceProps(std::vector<const char*> vk_validationlayers = { "VK_LAYER_KHRONOS_validation" },
			std::vector<const char*> vk_deviceextensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME })
			: vk_ValidationLayers(vk_validationlayers), vk_DeviceExtensions(vk_deviceextensions)
		{
		}
	};


	class YNM_API Instance 
	{
	public:
		virtual ~Instance() {}

		static Instance* Create(Window* m_Window, const InstanceProps& props = InstanceProps());

	};
}