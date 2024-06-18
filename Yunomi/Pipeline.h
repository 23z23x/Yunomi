#pragma once

#include "pch.h"
#include "Yunomi/Render/Shader.h"
#include <Yunomi/Render/Instance.h>

namespace ynm
{
	enum MSAALevel
	{
		MSAA_1 = 1,
		MSAA_2 = 2,
		MSAA_4 = 4,
		MSAA_8 = 8,
		MSAA_16 = 16,
		MSAA_32 = 32,
		MSAA_64 = 64
	};

	//Struct used to pass settings
	struct PipelineProps
	{
		Shader* Vertex;
		Shader* Fragment;

		//Settings
		MSAALevel msaaLevel;

		bool operator==(const PipelineProps& other) const {
			return std::tie(Vertex, Fragment, msaaLevel) == std::tie(other.Vertex, other.Fragment, other.msaaLevel);
		}

	};


	class Pipeline
	{
	public:
		virtual ~Pipeline() {}

		static Pipeline* Create(Instance* instance, PipelineProps props, uint32_t ID);

		inline PipelineProps getProps() { return props; }
		inline uint32_t getID() { return ID; }

	private:
		//This pipeline's properties
		PipelineProps props;
		
		uint32_t ID;
	};
}