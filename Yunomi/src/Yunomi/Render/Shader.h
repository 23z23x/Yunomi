#pragma once
#include "pch.h"

//Virtual interface class

namespace ynm
{
	//Enum to allow shader type to be declared in a platform independent way
	enum ShaderType
	{
		Vertex,
		TessellationControl,
		TesselationEvaluation,
		Geometry,
		Fragment,
		Compute
	};

	class YNM_API Shader
	{
	public:
		static Shader* Create(const std::string& filename, ShaderType type);

		virtual ~Shader() {}

		virtual void* getSpirv() const = 0;
		virtual ShaderType getYNMType() const = 0;
	};
}
