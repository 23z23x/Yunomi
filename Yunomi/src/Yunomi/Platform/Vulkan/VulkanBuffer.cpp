#include "pch.h"
#include "VulkanBuffer.h"
#include "VulkanInstance.h"

namespace ynm
{
	Buffer* Buffer::bufferRef = nullptr;
	BufferType Buffer::type = BufferType::VERTEX;

	VulkanChunk::VulkanChunk(VulkanInstance* instance, uint32_t size, uint32_t offset, void* data, VkBufferUsageFlagBits vkType)
		: size(size), offset(offset), instance(instance)
	{
		instance->CreateChunk(size, &(this->buffer), &(this->bufferMemory), &ID, data, vkType);
	}

	VulkanChunk::~VulkanChunk()
	{
		instance->DeleteChunk(buffer, bufferMemory, ID);
	}

	Buffer* Buffer::Create(Instance* instance, BufferType type)
	{
		VkBufferUsageFlagBits vkType;
		switch(type)
		{
		case BufferType::VERTEX:
			vkType = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			break;
		case BufferType::INDEX:
			vkType = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			break;
		default:
			vkType = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			break;
		}

		VulkanBuffer* buffer = new VulkanBuffer((VulkanInstance*)instance, vkType);
		bufferRef = buffer;
		type = type;

		return buffer;
	}

	uint32_t Buffer::CreateChunk(uint32_t size, uint32_t offset, void* data)
	{
		VulkanBuffer* buffer = (VulkanBuffer*)bufferRef;

		return buffer->CreateVulkanChunk(size, offset, data);
	}

	void Buffer::DeleteChunk(uint32_t ID)
	{
		VulkanBuffer* buffer = (VulkanBuffer*)bufferRef;

		buffer->DeleteVulkanChunk(ID);
	}

	VulkanBuffer::VulkanBuffer(VulkanInstance* instance, VkBufferUsageFlagBits vkType)
		: instance(instance), vkType(vkType)
	{

	}

	uint32_t VulkanBuffer::CreateVulkanChunk(uint32_t size, uint32_t offset, void* data)
	{
		VulkanChunk* chunk = new VulkanChunk(this->instance, size, offset, data, vkType);

		chunks.push_back(chunk);
		
		return chunk->getID();
	}

	void VulkanBuffer::DeleteVulkanChunk(uint32_t ID)
	{
		auto it = std::find_if(chunks.begin(), chunks.end(), [ID](const VulkanChunk* obj) {
			if (obj->getID() == ID)
			{
				obj->~VulkanChunk();
				return 0;
			}
			});
	}

	UniformBuffer* UniformBuffer::Create(Instance* instance)
	{
		return new VulkanUniformBuffer((VulkanInstance*)instance);
	}

	VulkanUniformBuffer::VulkanUniformBuffer(VulkanInstance* vkinstance)
	{
		vkinstance->createUniformBuffers(&(this->uniformBuffers), &(this->uniformBuffersMemory), &(this->uniformBuffersMapped));
		this->instance = vkinstance;
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		this->instance->destroyUniformBuffers(this->uniformBuffers, this->uniformBuffersMemory);
	}

}