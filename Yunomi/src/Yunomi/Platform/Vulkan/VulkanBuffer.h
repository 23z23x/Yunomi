#pragma once

#include "Yunomi/Render/Buffer.h"
#include "Yunomi/Platform/Vulkan/VulkanInstance.h"

#include "Yunomi/Render/Geometry.h"

namespace ynm
{
	class YNM_API VulkanChunk
	{
	public:
		VulkanChunk(VulkanInstance* instance, uint32_t size, std::vector<uint32_t> offsets, void* data, VkBufferUsageFlagBits vkType, BufferType type, uint32_t count);
		~VulkanChunk();

		inline VkBuffer getBuffer() const { return buffer; }
		inline VkDeviceMemory getMemory() const { return bufferMemory; }
		inline uint32_t getSize() const { return size; }
		inline std::vector<uint32_t> getOffsets() const { return offsets; }
		inline uint32_t getID() const { return ID; }
		inline uint32_t getCount() const { return count; }
	private:
		VulkanInstance* instance;

		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
		uint32_t size;
		std::vector<uint32_t> offsets;
		uint32_t ID;

		//Number of distinct items of type in buffer
		uint32_t count;
	};

	class VulkanBuffer : public Buffer
	{
	public:
		VulkanBuffer(VulkanInstance* instance, VkBufferUsageFlagBits vkType, BufferType type);
		~VulkanBuffer() {}

		uint32_t CreateVulkanChunk(uint32_t size, std::vector<uint32_t> offsets, void* data, uint32_t count);
		void DeleteVulkanChunk(uint32_t);

		inline std::vector<VulkanChunk*> getChunks() { return chunks; }
	private:
		std::vector<VulkanChunk*> chunks;
		VulkanInstance* instance;
		VkBufferUsageFlagBits vkType;
		BufferType type;

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
