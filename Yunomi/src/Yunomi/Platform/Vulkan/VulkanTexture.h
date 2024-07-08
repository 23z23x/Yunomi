#pragma once

#include "Yunomi/Render/Texture.h"
#include "Yunomi/Platform/Vulkan/VulkanInstance.h"

namespace ynm
{
	class VulkanTexture : public Texture
	{
	public:
		//Instance, filename to create texture from
		VulkanTexture(VulkanInstance* instance, std::string filename, uint32_t ID);
		~VulkanTexture();

		inline std::string getFilename() const override { return filename; }

		inline void* getImageView() const override { return (void*) &textureImageView; }
		inline void* getTextureSampler() const override { return (void*) &textureSampler; }
		
	private:
		uint32_t mipLevels;
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;

		VkImageView textureImageView;
		VkSampler textureSampler;

		std::string filename;
		VulkanInstance* instance;


	};

}