#pragma once
#include "pch.h"
#include "Instance.h"
#include "Window.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Geometry.h"
#include "Yunomi/Game_Objects/Object.h"

namespace ynm
{
	class Renderer
	{
	public:
		//Creates a Renderer object. Shaders are to be moved to a pipeline creation method when pipeline creation is decoupled from instance creation.
		Renderer(Window* window, Shader* vertex, Shader* fragment);
		~Renderer();

		//Load

		//Loads a vector of gameobjects to be rendered. Calling this again removes all current objects and replaces it with new objects.
		void LoadObjects(std::vector<GameObject> objects);
		void Renderer::AddDescriptors();
		//Creates a texture from filename.
		uint32_t CreateTexture(std::string filename);
		//Creates a mesh from a filename.
		Mesh CreateMesh(std::string filename);
		//Creates a mesh from given parameters
		Mesh CreateQuad(float x1, float x2, float y1, float y2, float depth);

		//Draw
		void StartDraw();
		void UpdateUniform();
		void EndDraw();

		//Utility
		void FrameResize();

	private:
		Instance* instance;
		Window* window;
		Buffer* vertexBuffer;
		Buffer* indexBuffer;
		Buffer* instanceBuffer;
		UniformBuffer* uniformBuffer;

		uint32_t nextTextureID = 0;
		uint32_t nextMeshID = 0;

		std::vector<Texture*> textures;

	};
}
