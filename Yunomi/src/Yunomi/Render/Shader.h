#pragma once
#include "pch.h"

//Virtual interface class

namespace ynm
{
	class YNM_API Shader
	{

		virtual ~Shader();

		static Shader* Create(const std::string& filename);

	};
}
