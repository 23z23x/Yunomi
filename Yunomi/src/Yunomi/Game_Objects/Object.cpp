#include "pch.h"
#include "Object.h"

namespace ynm
{
	Object::Object(uint32_t ID, std::string name, InstanceData instanceData)
	{
		this->ID = ID;
		this->name = name;
		this->instanceData = instanceData;
	}


	TexturedObject::TexturedObject(uint32_t ID, std::string name, InstanceData instanceData, uint32_t textureID)
		: Object(ID, name, instanceData)
	{
		this->textureID = textureID;
	}

	void TexturedObject::setTexture(uint32_t ID)
	{
		this->textureID = ID;
	}

	GameObject::GameObject(uint32_t ID, std::string name, InstanceData instanceData, uint32_t textureID, Mesh* shape)
		: TexturedObject(ID, name, instanceData, textureID)
	{
		this->shape = shape;
	}

	GameObject::~GameObject()
	{

	}
}