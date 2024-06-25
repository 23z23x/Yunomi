#pragma once
#include "pch.h"
#include "Yunomi/Render/Geometry.h"
#include "Yunomi/Render/Texture.h"
#include "glm/glm.hpp"

namespace ynm
{
	//Object Parent Class
	class Object
	{
	public:
		inline uint32_t getID() const { return ID; }
		inline std::string getName() const { return name; }
		inline InstanceData getInstanceData() const { return instanceData; }

		//Methods that apply transformations to InstanceData
		void Translate(glm::vec3 vector);
		void RotateX(float degrees);
		void RotateY(float degrees);
		void RotateZ(float degrees);
		void Scale(glm::vec3 vector);

		//void setPosition(glm::vec3);
	protected:
		Object(uint32_t ID, std::string name, InstanceData instanceData);

	private:
		uint32_t ID;
		std::string name;
		InstanceData instanceData;
	};

	class TexturedObject : public Object
	{
	public:
		inline uint32_t getTextureID() const { return textureID; }

		void setTexture(uint32_t ID);
	protected:
		TexturedObject(uint32_t ID, std::string name, InstanceData instanceData, uint32_t textureID);
	private:
		uint32_t textureID;
	};

	class GameObject : public TexturedObject
	{
	public:
		GameObject(uint32_t ID, std::string name, InstanceData instanceData, uint32_t textureID, Mesh* shape);
		~GameObject();

		inline Mesh* getShape() const { return shape; }
	private:
		Mesh* shape;
	};
}