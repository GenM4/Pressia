#pragma once

#include "Pressia.h"

namespace Pressia {
	class EditorLayer : public Layer {
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		OrthographicCameraController m_CameraController;

		//Temp
		Ref<Shader> m_Shader;
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_TextureMap;
		Ref<Texture2D> m_Sprite;
		Ref<Framebuffer> m_Framebuffer;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
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
}
