#include "Sandbox.h"

Sandbox::Sandbox() {

		uint32_t groundID = renderer->CreateTexture("C:/repos/Yunomi/Yunomi/src/Yunomi/TestAssets/ground.png");

		ynm::Mesh quadground = renderer->CreateQuad(-30.0f, 30.0f, 30.0f, -30.0f, 0.0f);

		ynm::InstanceData groundData;
		groundData.ID = 0;
		groundData.textureID = groundID;

		ynm::GameObject ground = ynm::GameObject("ground", groundData, &quadground);

		//ground.Translate(glm::vec3())

		std::vector<ynm::GameObject> objs;
		objs.push_back(ground);

		renderer->LoadObjects(objs);

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