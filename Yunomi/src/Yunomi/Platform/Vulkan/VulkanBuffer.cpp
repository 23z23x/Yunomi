#include "pch.h"
#include "VulkanBuffer.h"
#include "VulkanInstance.h"

namespace ynm
{
	VertexBuffer* VertexBuffer::Create(Instance* instance, std::vector<Vertex> vertices)
	{
		return new VulkanVertexBuffer((VulkanInstance*) instance, vertices);
	}

	IndexBuffer* IndexBuffer::Create(Instance* instance, std::vector<uint16_t> indices)
	{
		return new VulkanIndexBuffer((VulkanInstance*) instance, indices);
	}

	UniformBuffer* UniformBuffer::Create(Instance* instance)
	{
		return new VulkanUniformBuffer((VulkanInstance*) instance);
	}

	VulkanVertexBuffer::VulkanVertexBuffer(VulkanInstance* vkinstance, std::vector<Vertex> vertices)
	{
		vkinstance->createVertexBuffer(&(this->vertexBuffer), &(this->vertexBufferMemory), vertices);
		this->instance = vkinstance;
		this->verticesSize = vertices.size();
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		this->instance->destroyVertexBuffer(this->vertexBuffer, this->vertexBufferMemory);
	}

	VulkanIndexBuffer::VulkanIndexBuffer(VulkanInstance* vkinstance, std::vector<uint16_t> indices)
	{
		vkinstance->createIndexBuffer(&(this->indexBuffer), &(this->indexBufferMemory), indices);
		this->instance = vkinstance;
		this->indicesSize = indices.size();
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		this->instance->destroyIndexBuffer(this->indexBuffer, this->indexBufferMemory);
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