#pragma once

#include "pch.h"

namespace ynm
{
	//Forward declaration to avoid circular dependencies
	class Instance;

	class YNM_API Texture 
	{
	public:
		static Texture* Create(Instance* instance, std::string filename);

		virtual ~Texture() {};

		virtual std::string getFilename() const = 0;
		virtual void* getImageView() const = 0;
		virtual void* getTextureSampler() const = 0;

	};

}