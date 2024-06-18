#pragma once
#include "Pipeline.h"

#include <Yunomi/Platform/Vulkan/VulkanInstance.h>
#include <Yunomi/Platform/Vulkan/VulkanShader.h>
namespace ynm
{
	struct VulkanPipelineProps
	{
		VulkanShader* Vertex;
		VulkanShader* Fragment;

		VkSampleCountFlagBits msaaLevel;
	};


	class VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline(VulkanInstance* instance, VulkanPipelineProps props);
		~VulkanPipeline();

		inline VulkanPipelineProps getProps() { return props; }

	private:
		//Variables
		VkPipeline graphicsPipeline;
		VkPipelineLayout pipelineLayout;

		VulkanPipelineProps props;

		//Methods
		VkShaderModule createShaderModule(const std::vector<uint32_t>& code, VkDevice* device);
	};
}
