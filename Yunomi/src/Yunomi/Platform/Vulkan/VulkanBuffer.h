#pragma once

#include "Yunomi/Render/Buffer.h"
#include "Yunomi/Platform/Vulkan/VulkanInstance.h"

#include "Yunomi/Render/Geometry.h"

namespace ynm
{
	class YNM_API VulkanChunk
	{
	public:
		VulkanChunk(VulkanInstance* instance, uint32_t size, uint32_t offset, void* data, VkBufferUsageFlagBits vkType);
		~VulkanChunk();

		inline VkBuffer getBuffer() const { return buffer; }
		inline VkDeviceMemory getMemory() const { return bufferMemory; }
		inline uint32_t getSize() const { return size; }
		inline uint32_t getOffset() const { return offset; }
		inline uint32_t getID() const { return ID; }
	private:
		VulkanInstance* instance;

		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
		uint32_t size;
		uint32_t offset;
		uint32_t ID;
	};

	class VulkanBuffer : public Buffer
	{
	public:
		VulkanBuffer(VulkanInstance* instance, VkBufferUsageFlagBits vkType);
		~VulkanBuffer() {}

		uint32_t CreateVulkanChunk(uint32_t size, uint32_t offset, void* data);
		void DeleteVulkanChunk(uint32_t);

		inline std::vector<VulkanChunk*> getChunks() { return chunks; }
	private:
		std::vector<VulkanChunk*> chunks;
		VulkanInstance* instance;
		VkBufferUsageFlagBits vkType;

	};

	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(VulkanInstance* instance);
		~VulkanUniformBuffer();

		inline void* getBuffer() const { return (void*)&uniformBuffers; }
		inline void* getMemory() const { return (void*)&uniformBuffersMemory; }
		inline void* getMap() const { return (void*)&uniformBuffersMapped; }
	private:
		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<void*> uniformBuffersMapped;

		VulkanInstance* instance;

	};
}
