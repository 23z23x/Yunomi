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
		inline glm::vec3 getPosition() const { return position; }

		void setPosition(glm::vec3);
	protected:
		Object(uint32_t ID, std::string name, glm::vec3 position);

	private:
		uint32_t ID;
		std::string name;
		glm::vec3 position;
	};

	class TexturedObject : public Object
	{
	public:
		inline Texture* getTexture() const { return texture; }

		void setTexture(Texture* texture);
	protected:
		TexturedObject(uint32_t ID, std::string name, glm::vec3 position, Texture* texture);
	private:
		Texture* texture;
	};

	class Object2D : public TexturedObject
	{
	public:
		Object2D(uint32_t ID, std::string name, glm::vec3 position, Texture* texture, Quad* shape);
		~Object2D();

		inline Quad* getShape() const { return shape; }
	private:
		Quad* shape;
	};

	class Object3D : public TexturedObject
	{
	public:
		Object3D(uint32_t ID, std::string name, glm::vec3 position, Texture* texture, Mesh* shape);
		~Object3D();

		inline Mesh* getShape() const { return shape; }
	private:
		Mesh* shape;
	};
}