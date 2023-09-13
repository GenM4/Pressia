#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Pressia/Events/ApplicationEvent.h"

#include "Window.h"

namespace Pressia {

	class PRESSIA_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// Defined by client
	Application* CreateApplication();
} // namespace Pressia
