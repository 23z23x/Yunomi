#include "pch.h"
#include "Renderer.h"

namespace ynm
{
	Renderer::Renderer(Window* window, Shader* vertex, Shader* fragment)
		: window(window)
	{
		//Create instance
		this->instance = Instance::Create(window, vertex, fragment);

		this->vertexBuffer = Buffer::Create(this->instance, BufferType::VERTEX);
		this->indexBuffer = Buffer::Create(this->instance, BufferType::INDEX);
		this->instanceBuffer = Buffer::Create(this->instance, BufferType::INSTANCE);

		this->uniformBuffer = UniformBuffer::Create(this->instance);
	}

	Renderer::~Renderer()
	{

	}

	void Renderer::LoadObjects(std::vector<GameObject> objects)
	{
		//Vector of already added meshes
		std::vector<Mesh*> meshes;

		//Vector of vectors of InstanceData
		std::vector<std::vector<InstanceData>> instData;

		//For all game objects, if their mesh has not yet been added, add it. Then, add InstanceData to the proper index in the array
		for (const GameObject obj : objects)
		{
			int index = 0;
			bool found = false;
			if (meshes.size() > 0)
			{
				for (index = 0; index < meshes.size(); index++)
				{
					if (obj.getShape()->getID() == meshes[index]->getID())
					{
						found = true;
						break;
					}
				}
			}

			if (!found)
			{
				//Push mesh back into vector
				meshes.push_back(obj.getShape());
				//Push this object's InstanceData back into vector
				std::vector<InstanceData> newData;
				newData.push_back(obj.getInstanceData());
				instData.push_back(newData);
			}
			else
			{
				//Get index, then push this object's InstanceData at the index
				instData[index].push_back(obj.getInstanceData());
			}
		}

		//We need to flatten the mesh vertex and index attributes into single vectors of each, and store a vector of the size of each mesh's indices.
		std::vector<Vertex> vertices;
		std::vector<uint32_t> vertexSizes;
		std::vector<uint32_t> indices;
		std::vector<uint32_t> indexSizes;
		for (Mesh* mesh : meshes)
		{
			const auto& meshVertices = mesh->getVertices();
			const auto& meshIndices = mesh->getIndices();

			vertices.insert(vertices.end(), meshVertices.begin(), meshVertices.end());
			vertexSizes.push_back(mesh->getVertices().size());
			indices.insert(indices.end(), meshIndices.begin(), meshIndices.end());
			indexSizes.push_back(mesh->getIndices().size());
		}



		//Now, we take the vectors and use them to fill our buffers
		this->vertexBuffer->CreateChunk(vertices.size(), vertexSizes, (void*)vertices.data(), meshes.size());

		this->indexBuffer->CreateChunk(indices.size(), indexSizes, (void*)indices.data(), meshes.size());

		//For instances, we are creating a chunk for every element in the vector
		for (std::vector<InstanceData> vector : instData)
		{
			std::vector<uint32_t> placeholder;
			instanceBuffer->CreateChunk(vector.size(), placeholder, (void*) vector.data(), vector.size());
		}
	}

	void Renderer::AddDescriptors()
	{
		instance->AddDescriptors(uniformBuffer, textures[0]);
	}

	uint32_t Renderer::CreateTexture(std::string filename)
	{
		Texture* text = Texture::Create(this->instance, filename, nextTextureID);
		this->textures.push_back(text);
		this->nextTextureID++;
		return text->getID();
	}

	Mesh Renderer::CreateMesh(std::string filename)
	{
		Mesh mesh = Mesh(filename, nextMeshID);
		this->nextMeshID++;
		return mesh;
	}

	Mesh Renderer::CreateQuad(float x1, float x2, float y1, float y2, float depth)
	{
		Mesh quad = Mesh(x1, x2, y1, y2, depth, nextMeshID);
		this->nextMeshID++;
		return quad;
	}

	void Renderer::StartDraw()
	{
		this->instance->StartDraw(this->vertexBuffer, this->indexBuffer, this->instanceBuffer);
	}

	void Renderer::UpdateUniform()
	{
		this->instance->UpdateUniform(uniformBuffer);
	}

	void Renderer::EndDraw()
	{
		this->instance->EndDraw();
	}

	void Renderer::FrameResize()
	{
		this->instance->FrameResize();
	}
}