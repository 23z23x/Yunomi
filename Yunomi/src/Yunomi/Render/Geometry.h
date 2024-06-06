#pragma once

#include "pch.h"

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//This header defines all types of geometry, including generic mesh loading functions and simple square geometry for sprites.

namespace ynm
{
	//Vertex definition used in all geometry
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec2 texCoord;

		bool operator==(const Vertex& other) const {
			return pos == other.pos && texCoord == other.texCoord;
		}
	};
}

//Hash function 
namespace std {
	template<> struct hash<ynm::Vertex> {
		size_t operator()(ynm::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1)));
		}
	};
}

namespace ynm
{
	struct InstanceData
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);
	};

	//Class all geometry should inherit from
	class Geometry
	{
	public:
		inline std::vector<Vertex> getVertices() const { return vertices; }
		inline std::vector<uint32_t> getIndices() const { return indices; }
		inline uint32_t getID() const { return ID; }
	protected:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		uint32_t ID;
	};


	//Class for objects of arbitrary shape
	class Mesh : public Geometry
	{
	public:
		Mesh(std::string filename, uint32_t ID);
		Mesh(float x1, float x2, float y1, float y2, float depth, uint32_t ID);

		~Mesh();
	};

}