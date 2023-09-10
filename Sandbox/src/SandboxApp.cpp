#include <Pressia.h>

class Sandbox : public Pressia::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}

};

Pressia::Application* Pressia::CreateApplication() {
	return new Sandbox();
}