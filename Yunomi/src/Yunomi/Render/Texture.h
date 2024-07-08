#pragma once

#include "pch.h"

namespace ynm
{
	//Global variable defining total number of textures a graphics API needs to consider
	static uint8_t YNM_MAX_TEXTURES = 100;

	//Forward declaration to avoid circular dependencies
	class Instance;

	class YNM_API Texture 
	{
	public:
		static Texture* Create(Instance* instance, std::string filename, uint32_t ID);

		virtual ~Texture() {};

		virtual std::string getFilename() const = 0;
		virtual void* getImageView() const = 0;
		virtual void* getTextureSampler() const = 0;
		inline uint32_t getID() const { return ID; }

	protected:
		uint32_t ID;

	};

}