#include "Sandbox.h"

Sandbox::Sandbox() {

		/*uint32_t groundID = renderer->CreateTexture("C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/ground.png");

		ynm::Mesh quadground = renderer->CreateQuad(-30.0f, 30.0f, 30.0f, -30.0f, 0.0f);

		ynm::InstanceData groundData;
		groundData.ID = 0;
		groundData.textureID = groundID;

		ynm::GameObject ground = ynm::GameObject("ground", groundData, &quadground);

		std::vector<ynm::GameObject> objs;
		objs.push_back(ground);

		renderer->LoadObjects(objs);*/

		std::vector<std::string> test;
		test.push_back("C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/viking_room.png");

		uint32_t ID1 = objManager->CreateObject("C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/viking_room.obj", test, "vikingroom1");
		uint32_t ID2 = objManager->CreateObject("C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/viking_room.obj", test, "vikingroom2");
		uint32_t ID3 = objManager->CreateObject("C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/viking_room.obj", test, "vikingroom3");

		objManager->ChangeTranslateObject(ID1, glm::vec3(1.5, 1.5, 0));

		objManager->ChangeTranslateObject(ID2, glm::vec3(1.0, 0, 0));
		objManager->ChangeRotateObject(ID2, 0, 45.0f);

		objManager->ChangeTranslateObject(ID3, glm::vec3(0, 0, 2.0));
		objManager->ChangeScaleObject(ID3, glm::vec3(2.0, 2.0, 2.0));

		objManager->CreateScene();

		renderer->AddDescriptors();
	}

Sandbox::~Sandbox() 
{

}

void Sandbox::OnEvent(ynm::Event& e)
	{
		//YNM_CORE_INFO("{0}", e);

		ynm::KeyDownEvent* kd = nullptr;
		ynm::MouseMovedEvent* mm = nullptr;


		switch (e.GetEventType())
		{
		case ynm::EventType::WinClose:
			//mainLoop = false;
			break;
		case ynm::EventType::KeyDown:
			kd = (ynm::KeyDownEvent*)&e;
			renderer->ChangePosition(kd->GetKeyCode());
			break;
		case ynm::EventType::MouseMove:
			mm = (ynm::MouseMovedEvent*)&e;
			renderer->ChangeDirection(mm->GetX(), mm->GetY());
			break;
		}
	}

ynm::Application* ynm::CreateApplication() {
	return new Sandbox();
}