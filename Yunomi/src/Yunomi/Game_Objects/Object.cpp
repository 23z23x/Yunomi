#include "pch.h"
#include "Object.h"

namespace ynm
{
	Object::Object(uint32_t ID, std::string name, glm::vec3 position)
	{
		this->ID = ID;
		this->name = name;
		this->position = position;
	}

	void Object::setPosition(glm::vec3 position)
	{
		this->position = position;
	}



	TexturedObject::TexturedObject(uint32_t ID, std::string name, glm::vec3 position, Texture* texture)
		: Object(ID, name, position)
	{
		this->texture = texture;
	}

	void TexturedObject::setTexture(Texture* texture)
	{
		this->texture = texture;
	}



	Object2D::Object2D(uint32_t ID, std::string name, glm::vec3 position, Texture* texture, Quad* shape)
		: TexturedObject(ID, name, position, texture)
	{
		this->shape = shape;
	}

	Object2D::~Object2D()
	{

	}



	Object3D::Object3D(uint32_t ID, std::string name, glm::vec3 position, Texture* texture, Mesh* shape)
		: TexturedObject(ID, name, position, texture)
	{
		this->shape = shape;
	}

	Object3D::~Object3D()
	{

	}
}