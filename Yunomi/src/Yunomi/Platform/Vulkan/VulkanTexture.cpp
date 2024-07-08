#include "pch.h"
#include "VulkanTexture.h"

namespace ynm
{
	Texture* Texture::Create(Instance* instance, std::string filename, uint32_t ID)
	{
		return new VulkanTexture((VulkanInstance*) instance, filename, ID);
	}

	VulkanTexture::VulkanTexture(VulkanInstance* instance, std::string filename, uint32_t ID)
	{
		this->instance = instance;
		this->filename = filename;
		this->ID = ID;
		instance->createTexture(filename, &(this->textureImage), &(this->textureImageMemory), &(this->textureImageView), &(this->textureSampler), &(this->mipLevels));
	}

	VulkanTexture::~VulkanTexture()
	{
		this->instance->destroyTexture(this->textureImage, this->textureImageMemory, this->textureImageView, this->textureSampler);
	}
}