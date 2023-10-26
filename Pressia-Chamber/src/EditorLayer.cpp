#include "EditorLayer.h"

#include "Pressia/Scene/SceneSerializer.h"
#include "Pressia/Utils/PlatformUtils.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <chrono>

namespace Pressia {

	EditorLayer::EditorLayer() : Layer("Sandbox2D"), m_CameraController(1920.0f / 1080.0f) {
	}

	void EditorLayer::OnAttach() {
		PS_PROFILE_FUNCTION();

		m_Texture = Texture2D::Create("Assets/Textures/Penguin.png");
		m_TextureMap = Texture2D::Create("Assets/Textures/colored_tilemap.png");
		m_Sprite = Texture2D::CreateSubTexture("Assets/Textures/colored_tilemap.png", { 12, 7 }, { 8, 8 }, { 1, 1 }, { 1, 1 });

		FramebufferSpecification fbspec;
		fbspec.Width = 1920;
		fbspec.Height = 1080;
		m_Framebuffer = Framebuffer::Create(fbspec);

		m_CameraController.SetZoomLevel(10.0f);

		m_ActiveScene = CreateRef<Scene>();
		/*
		auto square = m_ActiveScene->CreateEntity("Green Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.2f, 0.7f, 0.5f, 0.5f });

		auto square2 = m_ActiveScene->CreateEntity("Blue Square");
		square2.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.2f, 0.5f, 0.7f, 1.0f });

		auto camera1 = m_ActiveScene->CreateEntity("Camera 1");
		camera1.AddComponent<CameraComponent>();

		auto camera2 = m_ActiveScene->CreateEntity("Camera 2");
		camera2.AddComponent<CameraComponent>().Camera.SetOrthographicSize(5.0f);

		m_ActiveScene->SetCamera(camera1);

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
		m_SHP.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach() {
		PS_PROFILE_FUNCTION();

	}

	void EditorLayer::OnUpdate(Timestep ts) {
		PS_PROFILE_FUNCTION();

		m_TPF = ts;

		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification(); m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.ResizeBounds(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		//Update
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		// Render
		if (m_ResetRenderStats)
			Renderer2D::ResetStats();

		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.105f, 0.11f, 1.0f });
		RenderCommand::Clear();
		m_ActiveScene->OnUpdate(ts);	// Update Scene
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender() {
		PS_PROFILE_FUNCTION();

		static bool p_open = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen) {
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else {
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Pressia Chamber", &p_open, window_flags);

		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
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


		ImGui::End();

		ImGui::Begin("Frame Rate");	//FPS Meter
		ImGui::Text("Frame Time: %5.3f ms", m_TPF * 1000.0f);
		ImGui::Text("Frame Rate: %.0f", 1 / m_TPF);
		ImGui::End();

		m_SHP.OnImGuiRender();

		ImGui::Begin("Settings");

		ImGui::End();


		ImGui::Begin("Renderer Statistics");
		if (m_ResetRenderStats)
			ImGui::Text("Showing Renderer Statistics per frame");
		else
			ImGui::Text("Showing Total Renderer Statistics");
		ImGui::Checkbox("Reset every frame", &m_ResetRenderStats);
		auto stats = Renderer2D::GetStats();
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad Count: %d", stats.QuadCount);
		ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
		ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());
		ImGui::End();


		//	Scene Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize)) {
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_CameraController.ResizeBounds(viewportPanelSize.x, viewportPanelSize.y);
		}

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2{ 0,1 }, ImVec2{ 1,0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::OnEvent(Event& e) {
		m_CameraController.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(PS_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e) {

		//	Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(PSKeyCode::LEFT_CONTROL) || Input::IsKeyPressed(PSKeyCode::RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(PSKeyCode::LEFT_SHIFT) || Input::IsKeyPressed(PSKeyCode::RIGHT_SHIFT);
		switch (e.GetKeyCode()) {
			case (int)PSKeyCode::N: {
				if (control)
					NewScene();
				break;
			}
			case (int)PSKeyCode::O: {
				if (control)
					OpenScene();
				break;
			}
			case (int)PSKeyCode::S: {
				if (control && shift)
					SaveSceneAs();
				break;
			}
		}

		return false;
	}

	void EditorLayer::NewScene() {
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SHP.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene() {
		std::string filepath = FileDialogs::OpenFile("Pressia Scene (*.pss)\0*.pss\0");
		if (!filepath.empty()) {
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SHP.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.DeserializeText(filepath);
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