#pragma once
#include "pch.h"
#include "Instance.h"
#include "Window.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Geometry.h"
#include "Yunomi/Game_Objects/Object.h"
#include "PipelineManager.h"

namespace ynm
{
	class YNM_API Renderer
	{
	public:
		//Creates a Renderer object. Shaders are to be moved to a pipeline creation method when pipeline creation is decoupled from instance creation
		Renderer(Window* window);
		~Renderer();

		//Pipeline
		uint32_t CreatePipeline(PipelineProps props);
		void SetPipeline(uint32_t ID);

		//Load

		//Loads a vector of gameobjects to be rendered. Calling this again removes all current objects and replaces it with new objects
		void LoadObjects(std::vector<GameObject> objects);
		//Adds the descriptors to be used
		void AddDescriptors();
		//Creates a texture from filename
		uint32_t CreateTexture(std::string filename);
		//Creates a mesh from a filename
		Mesh* CreateMesh(std::string filename);
		//Creates a quad from given parameters
		Mesh* CreateQuad(float x1, float x2, float y1, float y2, float depth);

		//Draw
		void StartDraw();
		void UpdateUniform();
		void EndDraw();

		void ChangeDirection(float xpos, float ypos);
		void ChangePosition(int code);

	private:
		Instance* instance;
		Window* window;
		Buffer* vertexBuffer;
		Buffer* indexBuffer;
		Buffer* instanceBuffer;
		UniformBuffer* uniformBuffer;
		UniformBufferObject* ubo;
		PipelineManager* pipelineManager;

		//IDs assigned to objects by the renderer
		uint32_t nextTextureID = 0;
		uint32_t nextMeshID = 0;

		//Vector of textures created
		std::vector<Texture*> textures;

		//Variables for camera

		glm::vec3 position = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 direction = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f) - this->position);
		glm::vec3 right;
		glm::vec3 up;

		float horizontalAngle = 0.0f;
		float verticalAngle = 0.0f;
		float mouseSpeed = 0.00005f;
		float speed = 0.005f;

		//A texture used when a texture ID cannot be found, or was not allocated.
		Texture* defaultText;

	};
}
