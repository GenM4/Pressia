#include <Pressia.h>

class ExampleLayer : public Pressia::Layer {
public:
	ExampleLayer() : Layer("Example") {
	}

	void OnUpdate() override {
		PS_INFO("ExampleLayer::Update");
	}

	void OnEvent(Pressia::Event& event) override {
		PS_TRACE("{0}", event);
	}
};

class Sandbox : public Pressia::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Pressia::ImGuiLayer());
	}

	~Sandbox() {

	}

};

Pressia::Application* Pressia::CreateApplication() {
	return new Sandbox();
}