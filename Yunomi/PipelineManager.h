#pragma once

#include "Pipeline.h"
//Maybe remove?
#include <Yunomi/Render/Instance.h>

namespace ynm
{
	class PipelineManager
	{
	public:
		//Constructor that takes the instance the pipelines will be created for
		PipelineManager(Instance* instance);
		~PipelineManager();

		//Creates a new pipeline with given props OR if a pipeline with given settings already exists, sets that pipeline as current
		uint32_t CreatePipeline(PipelineProps props);
		//Switches the pipeline
		void SetPipeline(uint32_t ID);

		inline Pipeline* getCurrentPipeline() { return pipelines[currID]; }

	private:
		Instance* instance;

		//Current pipeline's ID
		uint32_t currID;
		//Vector of created pipelines
		std::vector<Pipeline*> pipelines;

		//Next ID to be assigned
		uint32_t nextID = 0;
	};
}
