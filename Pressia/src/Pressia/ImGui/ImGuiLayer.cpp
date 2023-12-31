#include "pspch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "Pressia/Core/Application.h"

#include <GLFW/glfw3.h> // Temp
#include <glad/glad.h> // Temp

#include <ImGuizmo.h>

namespace Pressia {


	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {
	}

	void ImGuiLayer::OnAttach() {
		PS_PROFILE_IMGUI_FUNCTION();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Inter-VariableFont_slnt,wght.ttf", 16);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach() {
		PS_PROFILE_IMGUI_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e) {
		if (m_BlockEvents) {
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin() {
		PS_PROFILE_IMGUI_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End() {
		PS_PROFILE_IMGUI_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		//Render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
	void ImGuiLayer::SetDarkThemeColors() {
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.0751f, 0.075105f, 0.07511f, 1.0f };

		colors[ImGuiCol_Header] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.29f, 0.3f, 0.35f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.151f, 0.16f, 1.0f };

		colors[ImGuiCol_Button] = ImVec4{ 0.21f, 0.401f, 0.51f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.41f, 0.51f, 0.601f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.351f, 0.451f, 0.5501f, 1.0f };

		colors[ImGuiCol_FrameBg] = ImVec4{ 0.21f, 0.401f, 0.51f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.41f, 0.51f, 0.601f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.351f, 0.451f, 0.5501f, 1.0f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.17f, 0.171f, 0.171f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.21f, 0.301f, 0.41f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.221f, 0.251f, 0.351f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.14f, 0.141f, 0.141f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.21f, 0.201f, 0.21f, 1.0f };

		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.151f, 0.155f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.17f, 0.171f, 0.175f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.601f, 0.61f, 0.651f, 1.0f };



	}
}

