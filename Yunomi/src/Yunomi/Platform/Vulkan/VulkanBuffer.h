#pragma once

#include "Yunomi/Render/Buffer.h"
#include "Yunomi/Platform/Vulkan/VulkanInstance.h"

#include "Yunomi/Render/Geometry.h"

namespace ynm
{
	class YNM_API VulkanChunk
	{
	public:
		//Vulkan instance
		//size of chunk in bytes
		//offsets at which each unit of data is stored
		//raw data
		//Vulkan buffer type
		//Yunomi buffer type
		//Number of buffer items
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

		uint32_t size;
		std::vector<uint32_t> offsets;
		
		//Number of distinct items of type in buffer
		uint32_t count;
		
		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
		uint32_t ID;

	};

	class VulkanBuffer : public Buffer
	{
	public:
		//Instance, Vulkan type, Yunomi type
		VulkanBuffer(VulkanInstance* instance, VkBufferUsageFlagBits vkType, BufferType type);
		~VulkanBuffer() {}

		//Creates a chunk affililated with this buffer
		uint32_t CreateVulkanChunk(uint32_t size, std::vector<uint32_t> offsets, void* data, uint32_t count);
		void DeleteVulkanChunk(uint32_t);

		inline std::vector<VulkanChunk*> getChunks() { return chunks; }
	private:
		std::vector<VulkanChunk*> chunks;
		VulkanInstance* instance;
		VkBufferUsageFlagBits vkType;
		BufferType type;

	};

	//Uniform buffer class
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
