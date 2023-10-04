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
	Pressia::Ref<Pressia::Texture2D> m_Texture;
	Pressia::Ref<Pressia::Texture2D> m_TextureMap;
	Pressia::Ref<Pressia::Texture2D> m_Sprite;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec3 m_Quad1Pos = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_Quad2Pos = { 0.0f, 0.0f, 0.0f };
	float m_Quad1Angle = 0.0f;
	float m_Quad2Angle = 0.0f;
	glm::vec2 m_QuadScale = glm::vec2(1.0f);
	float m_TilingFactor = 1.0f;
	bool m_ResetRenderStats = false;
	float m_TPF = 0.0f;
	float m_StressTestBound = 1.0f;
};

