#pragma once

#include "Core.h"

namespace Pressia {

	class PRESSIA_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// Defined by client
	Application* CreateApplication();
}

