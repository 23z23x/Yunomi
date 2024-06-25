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

	void Object::Translate(glm::vec3 vector)
	{
		this->instanceData.modelMatrix = glm::translate(this->instanceData.modelMatrix, vector);
	}

	void Object::RotateX(float degrees)
	{
		float rads = glm::radians(degrees);
		glm::vec3 axis = glm::vec3(1.0f, 0.0f, 0.0f);

		this->instanceData.modelMatrix = glm::rotate(this->instanceData.modelMatrix, rads, axis);
	}

	void Object::RotateY(float degrees)
	{
		float rads = glm::radians(degrees);
		glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);

		this->instanceData.modelMatrix = glm::rotate(this->instanceData.modelMatrix, rads, axis);
	}

	void Object::RotateZ(float degrees)
	{
		float rads = glm::radians(degrees);
		glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f);

		this->instanceData.modelMatrix = glm::rotate(this->instanceData.modelMatrix, rads, axis);
	}

	void Object::Scale(glm::vec3 vector)
	{
		this->instanceData.modelMatrix = glm::scale(this->instanceData.modelMatrix, vector);
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