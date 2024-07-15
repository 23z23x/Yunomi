#include "pch.h"
#include "ObjectManager.h"

namespace ynm
{
	ObjectManager::ObjectManager(Renderer* renderer)
		: renderer(renderer)
	{
	}

	ObjectManager::~ObjectManager()
	{
	}

	uint32_t ObjectManager::createObject(std::string meshFile, std::vector<std::string> materials, std::string name)
	{
		//Basically, we are just asking the renderer to do a lot of stuff for us

		//Eventually, this will be a loop that has the renderer create all the needed materials, checking if they already exist
		uint32_t groundID = renderer->CreateTexture(materials[0]);

		ynm::Mesh mesh = renderer->CreateMesh(meshFile);

		ynm::InstanceData newData;
		newData.ID = nextID;
		nextID++;
		//Again, this will be changed when new multiple material system is implemented
		newData.textureID = groundID;

		//Use Object constructor
		ynm::GameObject newObj = ynm::GameObject(name, newData, &mesh);

		//Add new object to vector
		objects.push_back(newObj);

		return newObj.getID();
	}

}