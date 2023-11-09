#pragma once

#include "Pressia/Core/Core.h"

#include "Pressia/Core/Window.h"
#include "Pressia/Core/LayerStack.h"
#include "Pressia/Events/Event.h"
#include "Pressia/Events/ApplicationEvent.h"

#include "Pressia/ImGui/ImGuiLayer.h"

namespace Pressia {

	struct ApplicationCommandLineArgs {
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const {
			PS_CORE_ASSERT(index < Count, "Indexed non-existent cmd line argument");
			return Args[index];
		}
	};

	class PRESSIA_API Application {
	public:
		Application(const std::string& name = "Pressia App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		ApplicationCommandLineArgs m_CommandLineArgs;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		static Application* s_Instance;
	};

	// Defined by client
	Application* CreateApplication(ApplicationCommandLineArgs args);
}
