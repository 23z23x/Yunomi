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

	uint32_t ObjectManager::CreateObject(std::string meshFile, std::vector<std::string> materials, std::string name)
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

	//void ObjectManger::AddObject(uint32_t ID)
	//void ObjectManager::RemoveObject(uint32_t ID)

	uint32_t ObjectManager::CreateScene(std::string filename)
	{
		//Loop through all lines of the file, collect the necessary data, and create all of the objects
		std::vector<std::string> lines = readLines(filename);
		for (std::string line : lines)
		{
			//Split the string using |, get all info, create object
			std::stringstream ss(line);

			std::string token;
			std::vector<std::string> tokens;
			char delimiter = '|';

			while (getline(ss, token, delimiter))
			{
				tokens.push_back(token);
			}

			std::string objName = tokens[0];
			std::string meshName = tokens[1];
			std::string textName = tokens[2];

			//Quick hack that will be changed when new texture system is implemented
			std::vector<std::string> textures;
			textures.push_back(textName);

			CreateObject(meshName, textures, objName);

		}

		//Now that all objects are created, have them rendered.
		renderer->LoadObjects(this->objects);


	}

	uint32_t ObjectManager::CreateScene()
	{
		renderer->LoadObjects(this->objects);
	}

}