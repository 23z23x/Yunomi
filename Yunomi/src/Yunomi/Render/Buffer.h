#pragma once

#include "pch.h"

namespace ynm
{
	//Forward declaration to avoid circular dependencies
	class Instance;

	enum BufferType
	{
		VERTEX,
		INDEX,
		INSTANCE,
		UNIFORM
	};

	class YNM_API Buffer
	{
	public:
		static Buffer* Create(Instance* instance, BufferType type);
		virtual ~Buffer() {}

		virtual uint32_t CreateChunk(uint32_t size, uint32_t offset, void* data);
		virtual void DeleteChunk(uint32_t ID);
	private:
		static Buffer* bufferRef;
		static BufferType type;
	};

	class YNM_API UniformBuffer : public Buffer
	{
	public:
		static UniformBuffer* Create(Instance* instance);

		virtual ~UniformBuffer() {}

		virtual void* getBuffer() const = 0;
		virtual void* getMemory() const = 0;
		virtual void* getMap() const = 0;
	};
}