#include "pspch.h"
#include "Application.h"

#include "Pressia/Events/ApplicationEvent.h"
#include "Pressia/Log.h"

namespace Pressia {

	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::Run() {
		WindowResizeEvent e(1920, 1080);
		if (e.IsInCategory(EventCategoryApplication)) {
			PS_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput)) {
			PS_TRACE(e);
		}

		while (true);
	}

}