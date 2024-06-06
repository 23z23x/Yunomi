#include "pch.h"
#include "Geometry.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace ynm
{
	Mesh::Mesh(std::string filename, uint32_t ID)
	{
        this->ID = ID;

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        //Load geometry
        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str())) {
            throw std::runtime_error(warn + err);
        }

        //Unique vertices in shape
        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        //for all the shapes in the object (triangles)
        for (const auto& shape : shapes) {
            //For every index in the shape
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex{};

                //Set the position of a vertex to be the position in 3D space
                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };
                //Set the texture coordinate
                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                //If this vertex has never been seen before, add it to unique. Otherwise, just add this repeated vertex to indices
                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }
	}

    Mesh::Mesh(float x1, float x2, float y1, float y2, float depth, uint32_t ID)
    {
        this->ID = ID;

        if (x2 <= x1 || y1 <= y2)
        {
            YNM_CORE_ERROR("Renderer: A Quad object has been defined with invalid coordinates!");
        }

        indices = {
            0, 1, 2, 2, 3, 0,
        };

        vertices = {
        {{x1, y1, depth}, {1.0f, 0.0f}},
        {{x1, y2, depth}, {0.0f, 0.0f}},
        {{x2, y2, depth}, {0.0f, 1.0f}},
        {{x2, y1, depth}, {1.0f, 1.0f}},
        };
    }

    Mesh::~Mesh() {}

}