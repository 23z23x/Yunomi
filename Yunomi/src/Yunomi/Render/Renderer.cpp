#include "pch.h"
#include "Renderer.h"

namespace ynm
{
	Renderer::Renderer(Window* window)
		: window(window)
	{
		//Create instance
		this->instance = Instance::Create(window);

		//Create buffers
		this->vertexBuffer = Buffer::Create(this->instance, BufferType::VERTEX);
		this->indexBuffer = Buffer::Create(this->instance, BufferType::INDEX);
		this->instanceBuffer = Buffer::Create(this->instance, BufferType::INSTANCE);

		this->uniformBuffer = UniformBuffer::Create(this->instance);

		this->pipelineManager = new PipelineManager(this->instance);

		this->ubo = new UniformBufferObject();
	}

	Renderer::~Renderer()
	{

	}

	uint32_t Renderer::CreatePipeline(PipelineProps props)
	{
		return this->pipelineManager->CreatePipeline(props);
	}

	void Renderer::SetPipeline(uint32_t ID)
	{
		this->pipelineManager->SetPipeline(ID);
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
			//If there aren't any meshes, don't check
			if (meshes.size() > 0)
			{
				//For all meshes already seen, check if this object's mesh is in the list
				for (index = 0; index < meshes.size(); index++)
				{
					//Compare the IDs
					if (obj.getShape()->getID() == meshes[index]->getID())
					{
						found = true;
						break;
					}
				}
			}

			//If not found, put the mesh in the vector
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
		//For each mesh, do the above
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
		//Just call the instance descriptors method
		instance->AddDescriptors(uniformBuffer, textures);
	}

	uint32_t Renderer::CreateTexture(std::string filename)
	{
		//Call instance texture creation
		Texture* text = Texture::Create(this->instance, filename, nextTextureID);
		//Put the texture in the vector, and increment ID
		this->textures.push_back(text);
		this->nextTextureID++;
		//Return the ID
		return text->getID();
	}

	Mesh Renderer::CreateMesh(std::string filename)
	{
		//Use the Mesh constructor
		Mesh mesh = Mesh(filename, nextMeshID);
		//Incrememnt ID
		this->nextMeshID++;
		//Return the mesh
		return mesh;
	}

	Mesh Renderer::CreateQuad(float x1, float x2, float y1, float y2, float depth)
	{
		//Use the Mesh constructor for quads
		Mesh quad = Mesh(x1, x2, y1, y2, depth, nextMeshID);
		//Increment ID
		this->nextMeshID++;
		//Return the quad
		return quad;
	}

	void Renderer::StartDraw()
	{
		//Call instance method
		this->instance->StartDraw(this->vertexBuffer, this->indexBuffer, this->instanceBuffer);
	}

	void Renderer::UpdateUniform()
	{

		this->ubo->model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		this->ubo->view = glm::lookAt(
			this->position,
			this->position + this->direction,
			this->up
		);
		this->ubo->proj = glm::perspective(glm::radians(45.0f), window->GetWidth()/ (float)window->GetHeight(), 0.1f, 10.0f);

		//We do this because y is inverted in glm
		this->ubo->proj[1][1] *= -1;

		//Call instance method
		this->instance->UpdateUniform(uniformBuffer, ubo);

		this->window->ResetMousePosition();
	}

	void Renderer::EndDraw()
	{
		//Call instance method
		this->instance->EndDraw();
	}

	void Renderer::ChangeDirection(float xpos, float ypos)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		horizontalAngle += mouseSpeed * time * float(this->window->GetWidth() / 2 - xpos);
		verticalAngle += mouseSpeed * time * float(this->window->GetHeight() / 2 - ypos);

		this->direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		// Assuming worldUp is the global up direction (0, 1, 0)
		glm::vec3 worldUp = glm::vec3(0, 1, 0);

		// Calculate the right vector
		this->right = glm::normalize(glm::cross(this->direction, worldUp));

		// Recalculate the up vector
		this->up = glm::normalize(glm::cross(this->right, this->direction));
	}

	void Renderer::ChangePosition(int code)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		switch (code)
		{
		case InputCode::W:
			this->position += direction * time * speed;
			break;
		case InputCode::S:
			this->position -= direction * time * speed;
			break;
		case InputCode::A:
			this->position -= right * time * speed;
			break;
		case InputCode::D:
			this->position += right * time * speed;
			break;
		case InputCode::LFT_SHFT:
			this->position += up * time * speed;
			break;
		case InputCode::LFT_CTRL:
			this->position -= up * time * speed;
			break;
		}
	}
}