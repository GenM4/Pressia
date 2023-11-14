#include "EditorLayer.h"

#include "Pressia/Scene/SceneSerializer.h"
#include "Pressia/Utils/PlatformUtils.h"
#include "Pressia/Math/Math.h"

#include <chrono>

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>

namespace Pressia {

	extern const std::filesystem::path g_AssetPath;	//	TODO: Add project system

	EditorLayer::EditorLayer() : Layer("Sandbox2D") {
	}

	void EditorLayer::OnAttach() {
		PS_PROFILE_FUNCTION();

		//	Resource Init
		m_IconPlay = Texture2D::Create("Resources/SceneControls/icon_playbutton.png");
		m_IconStop = Texture2D::Create("Resources/SceneControls/icon_stopbutton.png");

		FramebufferSpecification fbspec;
		fbspec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbspec.Width = 1920;
		fbspec.Height = 1080;
		m_Framebuffer = Framebuffer::Create(fbspec);

		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1) {
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(m_ActiveScene);
			serializer.DeserializeText(sceneFilePath);
		}

		/*
		class CameraController : public ScriptableEntity {
		public:
			void OnCreate() { std::cout << "Created" << std::endl; }
			void OnUpdate(Timestep ts) {
				auto& tc = GetComponent<TransformComponent>();
				float speed = 5.0f;

				if (Input::IsKeyPressed(PSKeyCode::A))
					tc.Translation.x -= speed * ts;
				if (Input::IsKeyPressed(PSKeyCode::D))
					tc.Translation.x += speed * ts;
				if (Input::IsKeyPressed(PSKeyCode::S))
					tc.Translation.y -= speed * ts;
				if (Input::IsKeyPressed(PSKeyCode::W))
					tc.Translation.y += speed * ts;
				if (Input::IsKeyPressed(PSKeyCode::Q))
					tc.Rotation.z += speed * ts;
				if (Input::IsKeyPressed(PSKeyCode::E))
					tc.Rotation.z -= speed * ts;
			}
			void OnDestroy() {}
		};

		camera1.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		camera2.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		*/

		NewScene();
	}

	void EditorLayer::OnDetach() {
		PS_PROFILE_FUNCTION();

	}

	void EditorLayer::OnUpdate(Timestep ts) {
		PS_PROFILE_FUNCTION();

		m_EIP.OnUpdate(ts);

		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification(); m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		if (m_EIP.GetResetRenderStats())
			Renderer2D::ResetStats();

		// Render
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.105f, 0.11f, 1.0f });
		RenderCommand::Clear();

		m_Framebuffer->ClearColorAttachment(1, -1);	//	Clear EntityID attachment to -1

		switch (m_SceneState) {
			case SceneState::Edit:
				if (m_ViewportFocused)
					m_EditorCamera.OnUpdate(ts);

				m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);	//	Update Editor Scene

				break;
			case SceneState::Play:
				m_ActiveScene->OnUpdateRuntime(ts);	//	Update Runtime Scene
				break;
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender() {
		PS_PROFILE_FUNCTION();

		RenderDockSpace();
		m_SHP.OnImGuiRender();
		m_CBP.OnImGuiRender();
		m_EIP.OnImGuiRender();


		ImGui::Begin("Test Settings");
		ImGui::SliderInt("Render Target", &m_RenderTargetIndex, 0, 1);
		ImGui::End();


		//	Scene Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(m_RenderTargetIndex);
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowser_Item")) {
				const char* path = (const char*)payload->Data;
				OpenScene(g_AssetPath / path);
			}
			ImGui::EndDragDropTarget();
		}

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		//	Gizmos
		Entity selectedEntity = m_SHP.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1) {
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			//	Camera

			//	Runtime Camera
			//auto cameraEntity = m_ActiveScene->GetCamera();
			//const auto& camera = cameraEntity->GetComponent<CameraComponent>().Camera;
			//const glm::mat4& cameraProjection = camera.GetProjection();
			//glm::mat4 cameraView = glm::inverse(cameraEntity->GetComponent<TransformComponent>().GetTransform());

			//	Editor Camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			//	Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			//	Snapping
			bool snap = Input::IsKeyPressed(PSKeyCode::LEFT_CONTROL);
			float snapValue = 0.5f;	//	Snap to 0.5m for translation/scale
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;	//	Snap to 45deg for rotation

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing()) {
				glm::vec3 translation, scale;
				glm::quat rotation;

				Math::DecomposeTransform(transform, translation, rotation, scale);

				tc.Translation = translation;
				tc.Rotation = glm::eulerAngles(rotation);
				tc.Scale = scale;

			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End();

	}

	void EditorLayer::UI_Toolbar() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		ImGui::Begin("##UI_Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		Ref<Texture2D> icon = m_SceneState == SceneState::Edit ? m_IconPlay : m_IconStop;
		float size = ImGui::GetWindowHeight() - 10.0f;

		ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton("SceneState Control", (ImTextureID)icon->GetRendererID(), ImVec2(size, size))) {
			if (m_SceneState == SceneState::Edit) {
				OnScenePlay();
			}
			else if (m_SceneState == SceneState::Play) {
				OnSceneStop();
			}
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(1);
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e) {
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(PS_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(PS_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e) {

		//	Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(PSKeyCode::LEFT_CONTROL) || Input::IsKeyPressed(PSKeyCode::RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(PSKeyCode::LEFT_SHIFT) || Input::IsKeyPressed(PSKeyCode::RIGHT_SHIFT);

		if (m_SceneState == SceneState::Edit) {
			switch (e.GetKeyCode()) {
				case (int)PSKeyCode::N:
					if (control)
						NewScene();
					break;
				case (int)PSKeyCode::O:
					if (control)
						OpenScene();
					break;
				case (int)PSKeyCode::S:
					if (control && shift)
						SaveSceneAs();
					break;
				case (int)PSKeyCode::R:
					if (!ImGuizmo::IsUsing())
						m_GizmoType = ImGuizmo::ROTATE;
					break;
				case (int)PSKeyCode::T:
					if (!ImGuizmo::IsUsing())
						m_GizmoType = ImGuizmo::TRANSLATE;
					break;
				case (int)PSKeyCode::Y:
					if (!ImGuizmo::IsUsing())
						m_GizmoType = ImGuizmo::SCALE;
					break;
				case (int)PSKeyCode::C:
					if (control)
						m_SHP.CopySelectedEntity();
					break;
				case (int)PSKeyCode::V:
					if (control)
						m_SHP.PasteCopiedEntity();
					break;
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
		if (e.GetMouseButton() == (int)PSMouseCode::PS_MOUSE_BUTTON_LEFT) {
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(PSKeyCode::LEFT_CONTROL))	//	Implement CanPick() function with these checks
				m_SHP.SetSelectedEntity(m_HoveredEntity);
		}

		return false;
	}

	void EditorLayer::RenderDockSpace() {
		static bool p_open = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (m_FullscreenEditor) {
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else {
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Pressia Chamber", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (m_FullscreenEditor)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("Main Dockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}


		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New", "Ctrl+N")) {
					NewScene();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O")) {
					OpenScene();
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
					SaveSceneAs();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void EditorLayer::OnScenePlay() {
		m_SceneState = SceneState::Play;
		m_GizmoType = -1;
	}

	void EditorLayer::OnSceneStop() {
		m_SceneState = SceneState::Edit;
	}

	void EditorLayer::NewScene() {
		m_EditorCamera = EditorCamera(45.0f, 16.0f / 9.0f, 0.001f, 1000.0f);	//	Reset editor camera
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->CreateDefaultCamera();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SHP.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene() {
		std::string filepath = FileDialogs::OpenFile("Pressia Scene (*.pss)\0*.pss\0");
		if (!filepath.empty()) {
			OpenScene(filepath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path path) {
		if (path.extension() == ".pss") {
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SHP.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.DeserializeText(path.string());

			if (m_ActiveScene->GetCamera() == nullptr)
				m_ActiveScene->CreateDefaultCamera();
		}
	}

	void EditorLayer::SaveSceneAs() {
		std::string filepath = FileDialogs::SaveFile("Pressia Scene (*.pss)\0*.pss\0");
		if (!filepath.empty()) {
			SceneSerializer serializer(m_ActiveScene);
			serializer.SerializeText(filepath);
		}
	}

}