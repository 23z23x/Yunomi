#pragma once

#include "Yunomi/Render/Buffer.h"
#include "Yunomi/Platform/Vulkan/VulkanInstance.h"

namespace ynm
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(VulkanInstance* instance, std::vector<Vertex> vertices);
		~VulkanVertexBuffer();

		inline void* getBuffer() const override { return (void*) &vertexBuffer; }
		inline void* getMemory() const override { return (void*)&vertexBufferMemory; }
	private:
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		VulkanInstance* instance;

	};

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(VulkanInstance* instance, std::vector<uint32_t> vertices);
		~VulkanIndexBuffer();

		inline void* getBuffer() const override { return (void*)&indexBuffer; }
		inline void* getMemory() const override { return (void*)&indexBufferMemory; }
	private:
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

		VulkanInstance* instance;
	};

	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(VulkanInstance* instance);
		~VulkanUniformBuffer();

		inline void* getBuffer() const override { return (void*)&uniformBuffers; }
		inline void* getMemory() const override { return (void*)&uniformBuffersMemory; }
		inline void* getMap() const override { return (void*)&uniformBuffersMapped; }
	private:
		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<void*> uniformBuffersMapped;

		VulkanInstance* instance;

	};
}
