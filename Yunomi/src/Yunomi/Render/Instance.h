#pragma once
#include "pch.h"
#include "Window.h"
#include "Shader.h"
#include "glm/glm.hpp"

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

    struct Vertex {
        glm::vec2 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

            return attributeDescriptions;
        }
    };


	class YNM_API Instance 
	{
	public:
		virtual ~Instance() {}

		//In the future, might want to change this to take a struct where Shaders are more explicitly defined as their types.
		static Instance* Create(Window* m_Window, Shader* vertex, Shader* fragment, const InstanceProps& props = InstanceProps());

	};
}