#include "pch.h"
#include "VulkanTexture.h"

namespace ynm
{
	Texture* Texture::Create(Instance* instance, std::string filename)
	{
		return new VulkanTexture((VulkanInstance*) instance, filename);
	}

	VulkanTexture::VulkanTexture(VulkanInstance* instance, std::string filename)
	{
		this->instance = instance;
		this->filename = filename;
		instance->createTexture(filename, &(this->textureImage), &(this->textureImageMemory), &(this->textureImageView), &(this->textureSampler));
	}

	VulkanTexture::~VulkanTexture()
	{
		this->instance->destroyTexture(this->textureImage, this->textureImageMemory, this->textureImageView, this->textureSampler);
	}
}