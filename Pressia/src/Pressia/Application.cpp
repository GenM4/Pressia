#include "pspch.h"
#include "Application.h"

#include "Pressia/Events/ApplicationEvent.h"
#include "Pressia/Log.h"

namespace Pressia {

	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application() {

	}

	void Application::Run() {

		while (m_Running) {
			m_Window->OnUpdate();
		}
	}

}