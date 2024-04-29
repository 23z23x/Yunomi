#pragma once

#include "pch.h"

#include "Yunomi/Core.h"

//Interface to be implemented per API

namespace ynm
{
	class YNM_API Texture
	{
		static Texture* Create(const std::string filename);

		virtual ~Texture() {}

		virtual void* getTexture();
	};
}