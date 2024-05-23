#pragma once
#include "pch.h"
#include "Window.h"
#include "Shader.h"
#include "Geometry.h"

#include "vulkan/vulkan.h"
#include <vulkan/vulkan_core.h>

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
        //Might want to move this to VulkanInstance.h
		std::vector<const char*> vk_ValidationLayers;
		std::vector<const char*> vk_DeviceExtensions;

		InstanceProps(std::vector<const char*> vk_validationlayers = { "VK_LAYER_KHRONOS_validation" },
			std::vector<const char*> vk_deviceextensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME })
			: vk_ValidationLayers(vk_validationlayers), vk_DeviceExtensions(vk_deviceextensions)
		{
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

        //This method is used when the window changes size, in Vulkan it changes the swap chain.
        static void FrameResize();

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