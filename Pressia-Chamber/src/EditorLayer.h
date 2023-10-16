#pragma once

#include "Pressia.h"
#include "Panels/SceneHeirarchyPanel.h"

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

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_CameraEntity2;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		bool m_ResetRenderStats = false;
		float m_TPF = 0.0f;

		bool m_Camera1Selected = true;
		bool m_Camera2Selected = false;
		bool m_CameraSelectedFB[2] = { m_Camera1Selected, m_Camera2Selected };

		//	Panels
		SceneHeirarchyPanel m_SHP;
	};
}
