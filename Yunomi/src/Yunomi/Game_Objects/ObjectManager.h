#pragma once
#include "Yunomi/Render/Renderer.h"

namespace ynm
{
	//Class that creates and stores objects
	class ObjectManager
	{
	public:
		/*
		Arguments
			1) Renderer that should be used to make textures/meshes
		*/
		ObjectManager(Renderer* renderer);
		~ObjectManager();

		/*
		Arguments
			1) file to read mesh from
			2) vector of textures/materials to be used on the object (in object order)
			3) desired object name

		Description
			Creates an object using the given parameters.

		Return
			Returns obj ID if successful, -1 if error
		*/

		uint32_t CreateObject(std::string meshFile, std::vector<std::string> materials, std::string name);

		/*
		Arguments
			1) ID of object to add the scene (to be rendered)

		Description
			Adds the object with the given ID to the current scene
		*/

		//void AddObject(uint32_t ID);

		/*
		Arguments
			1) ID of the object to remove from the scene (to be rendered)

		Description
			Removes the object with the given ID from the current scene
		*/

		//void RemoveObject(uint32_t ID);

		/*
		Arguments
			1) name of a file to read from, which describes a scene. Each line should be in the following format
				Objname|Meshfilename|tex1filename| ... |texNfilename

				(Initial transform data to be added)

		Description
			Reads in a file written in the above format. All objects described are automatically created and added to render.

		Return
			-1 if error (filename not found, syntax error) or the number of objects created
		*/

		uint32_t CreateScene(std::string filename);

		//This overload just creates a scene with the objects already in the objects vector, for hardcoded testing
		uint32_t CreateScene();

		/*
		Arguments
			1) Object ID
			2) Translation vector

		Description
			Translates the specified object according to the provided vector
		*/

		void ChangeTranslateObject(uint32_t ID, glm::vec3 vector);

		/*
		Arguments
			1) Object ID
			2) Axis to translate on (0 = X, 1 = Y, 2 = Z)
			3) Degrees to rotate

		Description
			Rotates the specified object according to the provided parameters
		*/

		void ChangeRotateObject(uint32_t ID, uint32_t axis, float degrees);

		/*
		Arguments
			1) Object ID
			2) Translation vector

		Description
			Scales the specified object according to the provided vector
		*/

		void ChangeScaleObject(uint32_t ID, glm::vec3 vector);

		/*
		Arguments
			1) Object ID
			2) vector of filename strings

		Description
			Changes the materials associated with an object. Textures must have already been created (no dynamic loading of textures, load all needed textures at start)

			Same rules apply as Object creation, filenames should be in order specified by mesh file
		*/

		//void ChangeObjectMaterials(uint32_t ID, std::vector<std::string> filenames);

		//DATA

	private:

		std::vector<GameObject> objects;
		uint32_t nextID = 0;

		Renderer* renderer;
	};

}