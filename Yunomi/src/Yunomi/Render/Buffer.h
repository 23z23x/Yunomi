#pragma once

#include "pch.h"

namespace ynm
{
	//Forward declaration to avoid circular dependencies
	class Instance;

	//Buffer type enum
	enum BufferType
	{
		VERTEX,
		INDEX,
		INSTANCE,
		UNIFORM
	};

	//Buffer class
	class YNM_API Buffer
	{
	public:
		//Constructor takes the instance to create the buffer with, and the buffer type
		static Buffer* Create(Instance* instance, BufferType type);
		virtual ~Buffer() {}

		//Used to create a buffer
		virtual uint32_t CreateChunk(uint32_t size, std::vector<uint32_t> offsets, void* data, uint32_t count);
		virtual void DeleteChunk(uint32_t ID);
	protected:
		//Needed to invoke the used rendering API's version of specific methods
		Buffer* bufferRef;
		//Type stored
		BufferType type;
	};

	//Specific uniform buffer type
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