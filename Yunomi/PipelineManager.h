#pragma once

#include <Yunomi/Render/Pipeline.h>
//Maybe remove?
#include <Yunomi/Render/Instance.h>

namespace ynm
{
	class PipelineManager
	{
	public:
		//Constructor that takes the instance the pipelines will be created for
		PipelineManager(Instance instance);
		~PipelineManager();

		//Creates a new pipeline with given props OR if a pipeline with given settings already exists, sets that pipeline as current
		uint32_t CreatePipeline(PipelineProps props);
		//Switches the pipeline
		inline void SwitchPipeline(uint32_t ID) { currID = ID; }

		inline Pipeline* getCurrentPipeline() { return pipelines[currID]; }

	private:
		//Current pipeline's ID
		uint32_t currID;
		//Vector of created pipelines
		std::vector<Pipeline*> pipelines;

		//Helper function that returns true when the pipelines have identical settings
		bool CompareProps(PipelineProps x, PipelineProps y);
	};
}
