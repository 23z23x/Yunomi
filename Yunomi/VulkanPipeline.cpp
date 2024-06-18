#include "pch.h"
#include "VulkanPipeline.h"

namespace ynm
{
	Pipeline* Pipeline::Create(Instance* instance, PipelineProps props, uint32_t ID)
	{
		ID = ID;

		VulkanInstance* vkInstance = (VulkanInstance*)instance;
		VulkanPipelineProps vkProps;

		vkProps.Vertex = (VulkanShader*)props.Vertex;
		vkProps.Fragment = (VulkanShader*)props.Fragment;

		switch (props.msaaLevel)
		{
		case MSAA_1:
			vkProps.msaaLevel = VK_SAMPLE_COUNT_1_BIT;
			break;
		case MSAA_2:
			vkProps.msaaLevel = VK_SAMPLE_COUNT_2_BIT;
			break;
		case MSAA_4:
			vkProps.msaaLevel = VK_SAMPLE_COUNT_4_BIT;
			break;
		case MSAA_8:
			vkProps.msaaLevel = VK_SAMPLE_COUNT_8_BIT;
			break;
		case MSAA_16:
			vkProps.msaaLevel = VK_SAMPLE_COUNT_16_BIT;
			break;
		case MSAA_32:
			vkProps.msaaLevel = VK_SAMPLE_COUNT_32_BIT;
			break;
		case MSAA_64:
			vkProps.msaaLevel = VK_SAMPLE_COUNT_64_BIT;
			break;
		}

		return new VulkanPipeline(vkInstance, vkProps);

	}

	VulkanPipeline::VulkanPipeline(VulkanInstance* instance, VulkanPipelineProps props)
		: props(props)
	{
        //Get the source for the shader modules
        VkShaderModule vertShaderModule = createShaderModule(props.Vertex->getSpirv(), instance->getDevice());
        VkShaderModule fragShaderModule = createShaderModule(props.Fragment->getSpirv(), instance->getDevice());

        //Set the stage info for both shaders
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

        //Set up binding descriptions
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        std::array<VkVertexInputBindingDescription, 2> bindingDescriptions = instance->VkgetBindingDescriptions();

        //get attribute descriptions

        auto attributeDescriptions = instance->VkgetAttributeDescriptions();

        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        //set input assembly
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        //set viewport state
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        //rasterizer settings
        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        //MSAA settings
        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = props.msaaLevel;

        //Depth stencil
        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable = VK_FALSE;

        //Color blending options
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        //Which states are dynamic
        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = instance->getDescriptorSetLayout();

        if (vkCreatePipelineLayout(*(instance->getDevice()), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            YNM_CORE_ERROR("Vulkan: Failed to create pipeline layout!");
            throw std::runtime_error("");
        }
        YNM_CORE_INFO("Vulkan: Successfully created pipeline layout!");

        //Region of the framebuffer the output will be rendered to (0, 0)
        //VkViewport viewport{};
        //viewport.x = 0.0f;
        //viewport.y = 0.0f;
        //viewport.width = (float)swapChainExtent.width;
        //viewport.height = (float)swapChainExtent.height;
        //viewport.minDepth = 0.0f;
        //viewport.maxDepth = 1.0f;

        //Scissor defines in what regions pixels will be stored. To go things displayed normally, should be the resolution
        //VkRect2D scissor{};
        //scissor.offset = { 0, 0 };
        //scissor.extent = swapChainExtent;

        //Dynamic states, the very few things we say can change about the pipeline without rebuilding
        //Here we make the viewport and scissor dynamic, so we can support resizing later


        //Tie everything together here
        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = *(instance->getRenderPass());
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        //Can create a new pipeline easily from this one if you give it a handle and an index


        if (vkCreateGraphicsPipelines(*(instance->getDevice()), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
            YNM_CORE_ERROR("Vulkan: Failed to create graphics pipeline!");
            throw std::runtime_error("");
        }

        YNM_CORE_INFO("Vulkan: Successfully created graphics pipeline!");

        vkDestroyShaderModule(*(instance->getDevice()), fragShaderModule, nullptr);
        vkDestroyShaderModule(*(instance->getDevice()), vertShaderModule, nullptr);
	}


	VkShaderModule VulkanPipeline::createShaderModule(const std::vector<uint32_t>& code, VkDevice* device)
	{
		//Shader struct
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size() * sizeof(uint32_t);
		createInfo.pCode = code.data();

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(*device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			YNM_CORE_ERROR("Vulkan: Failed to create shader module!");
			throw std::runtime_error("");
		}
		YNM_CORE_INFO("Vulkan: Successfully created shader module!");

		return shaderModule;
	}

}