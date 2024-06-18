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

	void PipelineManager::SetPipeline(uint32_t ID)
	{
		//provides pipeline object associated with ID
		auto it = std::find_if(pipelines.begin(), pipelines.end(), [ID](Pipeline* obj) {
			return obj->getID() == ID;
			});

		if (it != pipelines.end()) {
			Pipeline* foundPipeline = *it;
			this->instance->SetPipeline(foundPipeline);
		}
		else {
			YNM_CORE_ERROR("Pipeline with ID {0} not found.", ID);
		}

		this->currID = ID;
	}
}