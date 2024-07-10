#pragma once
#include "pch.h"
#include "Yunomi/Render/Geometry.h"
#include "Yunomi/Render/Texture.h"
#include "glm/glm.hpp"

namespace ynm
{
	//Struct that contains all info kept in the instance buffer for an instance
	struct YNM_API InstanceData
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		uint32_t textureID;
		uint32_t ID;
	};

	//Object Parent Class
	class YNM_API Object
	{
	public:
		inline uint32_t getID() const { return instanceData.ID; }
		inline uint32_t getTextureID() const { return instanceData.textureID; }
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
		Object(std::string name, InstanceData instanceData);

	private:
		std::string name;
		InstanceData instanceData;
	};

	class YNM_API GameObject : public Object
	{
	public:
		GameObject(std::string name, InstanceData instanceData, Mesh* shape);
		~GameObject();

		inline Mesh* getShape() const { return shape; }
	private:
		Mesh* shape;
	};
}