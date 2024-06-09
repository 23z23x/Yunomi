#include "pch.h"
#include "VulkanBuffer.h"
#include "VulkanInstance.h"

namespace ynm
{
	VulkanChunk::VulkanChunk(VulkanInstance* instance, uint32_t size, std::vector<uint32_t> offsets, void* data, VkBufferUsageFlagBits vkType, BufferType type, uint32_t count)
		: size(size), offsets(offsets), instance(instance), count(count)
	{
		instance->CreateChunk(size, &(this->buffer), &(this->bufferMemory), &ID, data, vkType, type);
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
		case BufferType::INSTANCE:
			vkType = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			break;
		case BufferType::UNIFORM:
			vkType = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			break;
		default:
			vkType = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			break;
		}

		VulkanBuffer* buffer = new VulkanBuffer((VulkanInstance*)instance, vkType, type);
		buffer->bufferRef = buffer;
		type = type;

		return buffer;
	}

	uint32_t Buffer::CreateChunk(uint32_t size, std::vector<uint32_t> offsets, void* data, uint32_t count)
	{
		VulkanBuffer* buffer = (VulkanBuffer*)bufferRef;

		return buffer->CreateVulkanChunk(size, offsets, data, count);
	}

	void Buffer::DeleteChunk(uint32_t ID)
	{
		VulkanBuffer* buffer = (VulkanBuffer*)bufferRef;

		buffer->DeleteVulkanChunk(ID);
	}

	VulkanBuffer::VulkanBuffer(VulkanInstance* instance, VkBufferUsageFlagBits vkType, BufferType type)
		: instance(instance), vkType(vkType), type(type)
	{

	}

	uint32_t VulkanBuffer::CreateVulkanChunk(uint32_t size, std::vector<uint32_t> offsets, void* data, uint32_t count)
	{
		VulkanChunk* chunk = new VulkanChunk(this->instance, size, offsets, data, vkType, type, count);

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