#pragma once
#include "pch.h"

//Virtual interface class

namespace ynm
{
	//Enum to allow shader type to be declared in a platform independent way
	enum ShaderType
	{
		VRTX,
		TESSCTRL,
		TESSEVAL,
		GMTRY,
		FRAG,
		COMP
	};

	class YNM_API Shader
	{
	public:
		static Shader* Create(const std::string& filename, ShaderType type);

		virtual ~Shader() {}

		virtual std::vector<uint32_t> getSpirv() const = 0;
		virtual ShaderType getYNMType() const = 0;
	};
}
