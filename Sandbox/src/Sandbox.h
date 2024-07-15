#pragma once
#include <Yunomi.h>

class Sandbox : public ynm::Application
{
public:
	Sandbox();
	~Sandbox();

	void OnEvent(ynm::Event& e) override;
};

ynm::Application* ynm::CreateApplication();