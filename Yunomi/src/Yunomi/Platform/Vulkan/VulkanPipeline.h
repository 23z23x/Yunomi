#pragma once
#include "Yunomi/Render/Pipeline.h"

#include <Yunomi/Platform/Vulkan/VulkanShader.h>
namespace ynm
{
	//forward delcarations
	class VulkanInstance;

	struct VulkanPipelineProps
	{
		VkExtent2D swapChainExtent;

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
		inline VkPipeline* getPipeline() { return &graphicsPipeline; }
		inline VkPipelineLayout* getPipelineLayout() { return &pipelineLayout; }
		inline VkRenderPass* getRenderPass() { return &renderPass; }
		inline VkImageView* getDepthImageView() { return &depthImageView; }
		inline VkImageView* getColorImageView() { return &colorImageView; }

	private:
		//Variables
		VkPipeline graphicsPipeline;
		VkPipelineLayout pipelineLayout;

		VkRenderPass renderPass;

		//Depth buffering
		VkImage depthImage;
		VkDeviceMemory depthImageMemory;
		VkImageView depthImageView;

		//Images for MSAA
		VkImage colorImage;
		VkDeviceMemory colorImageMemory;
		VkImageView colorImageView;

		VulkanPipelineProps props;

		VulkanInstance* instance;

		//Methods
		VkShaderModule createShaderModule(const std::vector<uint32_t>& code, VkDevice* device);

		void createRenderPass();

		void createDepthResources();
		void createColorResources();
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat findDepthFormat();
		bool hasStencilComponent(VkFormat format);
	};
}
