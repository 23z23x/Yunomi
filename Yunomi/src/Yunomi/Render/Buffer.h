#pragma once

#include "pch.h"

#include "Yunomi/Core.h"

namespace ynm
{
	//Forward declaration to avoid circular dependencies
	class Instance;
	struct Vertex;

	class YNM_API VertexBuffer
	{
	public:
		static VertexBuffer* Create(Instance* instance, std::vector<Vertex> vertices);

		virtual ~VertexBuffer() {}

		VertexBuffer() {}

		virtual void* getBuffer() const = 0;
		virtual void* getMemory() const = 0;


	};

	class YNM_API IndexBuffer
	{
	public:
		static IndexBuffer* Create(Instance* instance, std::vector<uint32_t> indices);

		virtual ~IndexBuffer() {}

		virtual void* getBuffer() const = 0;
		virtual void* getMemory() const = 0;
	};

	class YNM_API UniformBuffer
	{
	public:
		static UniformBuffer* Create(Instance* instance);

		virtual ~UniformBuffer() {}

		virtual void* getBuffer() const = 0;
		virtual void* getMemory() const = 0;
		virtual void* getMap() const = 0;

	};
}