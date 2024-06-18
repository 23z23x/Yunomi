#include "pch.h"
#include "PipelineManager.h"

namespace ynm
{
	PipelineManager::PipelineManager(Instance* instance)
		: instance(instance)
	{

	}

	PipelineManager::~PipelineManager()
	{

	}

	uint32_t PipelineManager::CreatePipeline(PipelineProps props)
	{
		//First, cycle through to see if the pipeline matches an existing one
		for (Pipeline* pipeline : pipelines)
		{
			if (props == pipeline->getProps())
			{
				return pipeline->getID();
			}
		}

		//Else, create a new pipeline, add it to the vector, increase nextID, and return the pipeline's ID
		Pipeline* newPipeline = Pipeline::Create(this->instance, props, nextID);
		pipelines.push_back(newPipeline);
		this->nextID++;
		return newPipeline->getID();
	}
}