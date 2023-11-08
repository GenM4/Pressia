#pragma once

#include "Pressia.h"
#include "Panels/SceneHeirarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/EngineInfoPanel.h"
#include "Pressia/Renderer/EditorCamera.h"

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
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void RenderDockSpace();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path path);
		void SaveSceneAs();
	private:
		EditorCamera m_EditorCamera;

		//	Temp
		Ref<Framebuffer> m_Framebuffer;
		int m_RenderTargetIndex = 0;

		Ref<Scene> m_ActiveScene;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		Entity m_HoveredEntity;

		//	Panels
		bool m_FullscreenEditor = true;
		SceneHeirarchyPanel m_SHP;
		ContentBrowserPanel m_CBP;
		EngineInfoPanel m_EIP;

		//	Gizmos
		int m_GizmoType = -1;	//	-1 = no gizmo
	};
}
