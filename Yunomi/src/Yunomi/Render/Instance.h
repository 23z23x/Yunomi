#pragma once
#include "pch.h"
#include "Window.h"
#include "Shader.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Vritual interface class

namespace ynm
{
    //Forward declaration to avoid circular dependencies
    class VertexBuffer;
    class IndexBuffer;
    class UniformBuffer;
    class Texture;


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

    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

	class YNM_API Instance 
	{
	public:
		virtual ~Instance() {}

		//In the future, might want to change this to take a struct where Shaders are more explicitly defined as their types.
        // 
        //Also, obviously right now Pipeline creation is tied to instance creation. I think I want to force the creation of at least
        //one pipeline at instance creation and then later allow the creation of additional pipelines
		static Instance* Create(Window* m_Window, Shader* vertex, Shader* fragment, const InstanceProps& props = InstanceProps());
        //Temporary method that sets Vulkan descriptors. As the renderer develops this is certainly not how things will work.
        //
        //Plan is to eventually pass this a struct with all of the Uniform Buffers and Textures that will be used. That way additional info
        //can be passed if needed in the future. 
        static void AddDescriptors(UniformBuffer* ub, Texture* tx);

        //Finally, the methods that actually get the instance to do something.

        //Method that starts recording of commands to be drawn
        static void StartDraw(VertexBuffer* vb, IndexBuffer* ib);

        //Method that updates a Uniform Buffer (For now it does a predetermined transformation, later it will be abstracted to do different ones.)
        static void UpdateUniform(UniformBuffer* ub);
        
        //Method that ends recording of commands to be drawn
        static void EndDraw();
    private:
        //Sort of a reference to itself. For member methods other than create, they need to invoke their instance's version of the methods
        //this is done by referencing that instance.
        static Instance* instanceref;
	};
}