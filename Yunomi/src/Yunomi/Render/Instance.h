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

		InstanceProps(std::vector<const char*> vk_validationlayers = { "VK_LAYER_KHRONOS_validation" })
			: vk_ValidationLayers(vk_validationlayers)
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