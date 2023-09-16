#include <Pressia.h>

class ExampleLayer : public Pressia::Layer {
public:
	ExampleLayer() : Layer("Example") {
	}

	void OnUpdate() override {
		//PS_INFO("ExampleLayer::Update");
	}

	void OnEvent(Pressia::Event& event) override {
		// PS_TRACE("{0}", event);

		if (Pressia::Input::IsKeyPressed(PS_KEY_T))
			PS_INFO("T KEY PRESSED");
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