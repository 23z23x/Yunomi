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


	TexturedObject::TexturedObject(uint32_t ID, std::string name, InstanceData instanceData, Texture* texture)
		: Object(ID, name, instanceData)
	{
		this->texture = texture;
	}

	void TexturedObject::setTexture(Texture* texture)
	{
		this->texture = texture;
	}



	Object2D::Object2D(uint32_t ID, std::string name, InstanceData instanceData, Texture* texture, Quad* shape)
		: TexturedObject(ID, name, instanceData, texture)
	{
		this->shape = shape;
	}

	Object2D::~Object2D()
	{

	}



	Object3D::Object3D(uint32_t ID, std::string name, InstanceData instanceData, Texture* texture, Mesh* shape)
		: TexturedObject(ID, name, instanceData, texture)
	{
		this->shape = shape;
	}

	Object3D::~Object3D()
	{

	}
}