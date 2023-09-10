#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Pressia {

	class PRESSIA_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// Defined by client
	Application* CreateApplication();
} // namespace Pressia
