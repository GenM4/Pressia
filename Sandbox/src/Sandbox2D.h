#pragma once

#include "Pressia.h"

class Sandbox2D : public Pressia::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Pressia::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Pressia::Event& event) override;

private:
	Pressia::OrthographicCameraController m_CameraController;

	//Temp
	Pressia::Ref<Pressia::VertexArray> m_SquareVA;
	Pressia::Ref<Pressia::Shader> m_Shader;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};

