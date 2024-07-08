#pragma once
#include "pch.h"
#include "Window.h"
#include "Shader.h"
#include "Geometry.h"


//Vritual interface class

namespace ynm
{
    //Forward declaration to avoid circular dependencies
    class Buffer;
    class UniformBuffer;
    class Texture;
    class Pipeline;

    //Right now, InstanceProps is not used for anything. Kept as an argument for future use.
	struct InstanceProps
	{
		std::vector<const char*> validationLayers;
		std::vector<const char*> deviceExtensions;

        //Defaults are for Vulkan, since it will work on most platforms
		InstanceProps(std::vector<const char*> validationlayers = { "VK_LAYER_KHRONOS_validation" },
			std::vector<const char*> deviceextensions = { "VK_KHR_SWAPCHAIN_EXTENSION_NAME" })
			: validationLayers(validationlayers), deviceExtensions(deviceextensions)
		{
		}
	};

    //This to be moved to the Buffer.h file
    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

	class YNM_API Instance 
	{
	public:
		virtual ~Instance() {}

		//Creates an Instance of whatever rendering API has been selected.
		static Instance* Create(Window* m_Window, const InstanceProps& props = InstanceProps());

        //Sets a pipeline as the current pipeline
        static void SetPipeline(Pipeline* pipeline);

        //Temporary method that sets Vulkan descriptors. As the renderer develops this is certainly not how things will work.
        //
        //Plan is to eventually pass this a struct with all of the Uniform Buffers and Textures that will be used. That way additional info
        //can be passed if needed in the future. 
        static void AddDescriptors(UniformBuffer* ub, std::vector<Texture*> tx);

        //Draw methods

        //Method that starts recording of commands to be drawn
        static void StartDraw(Buffer* vertex, Buffer* index, Buffer* instance);

        //Method that updates the Uniform Buffer
        static void UpdateUniform(UniformBuffer* ub, UniformBufferObject* ubo);
        
        //Method that ends recording of commands to be drawn
        static void EndDraw();
    private:
        //Sort of a reference to itself. For member methods other than create, they need to invoke their instance's version of the methods
        //this is done by referencing that rendering API's instance.
        static Instance* instanceref;
	};
}