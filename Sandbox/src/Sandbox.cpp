#include <Yunomi.h>

class Sandbox : public ynm::Application
{
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

ynm::Application* ynm::CreateApplication() {
	return new Sandbox();
}