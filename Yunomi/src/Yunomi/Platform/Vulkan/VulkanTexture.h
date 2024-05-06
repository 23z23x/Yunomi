#pragma once

#include "Yunomi/Render/Texture.h"
#include "Yunomi/Platform/Vulkan/VulkanInstance.h"

namespace ynm
{
	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture(VulkanInstance* instance, std::string filename);
		~VulkanTexture();

		inline std::string getFilename() const override { return filename; }

		inline VkImageView getImageView() { return textureImageView; }
		inline VkSampler getImageSampler() { return textureSampler; }
		
	private:
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;

		VkImageView textureImageView;
		VkSampler textureSampler;

		std::string filename;
		VulkanInstance* instance;


	};

}