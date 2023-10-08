#pragma once

#include "Core.h"

#include "Window.h"
#include "Pressia/Core/LayerStack.h"
#include "Pressia/Events/Event.h"
#include "Pressia/Events/ApplicationEvent.h"

#include "Pressia/Core/TimeStep.h"

#include "Pressia/ImGui/ImGuiLayer.h"

#include "Pressia/Renderer/Buffer.h"
#include "Pressia/Renderer/Shader.h"
#include "Pressia/Renderer/VertexArray.h"
#include "Pressia/Renderer/OrthographicCamera.h"

namespace Pressia {

	class PRESSIA_API Application {
	public:
		Application(const std::string& name = "Pressia App");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		void Close();
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		static Application* s_Instance;
	};

	// Defined by client
	Application* CreateApplication();
} // namespace Pressia
