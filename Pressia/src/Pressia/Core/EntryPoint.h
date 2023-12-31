#pragma once

#include "Application.h"

#ifdef PS_PLATFORM_WINDOWS

extern Pressia::Application* Pressia::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv) {

	Pressia::Log::Init();

	PS_PROFILE_BEGIN_SESSION("Startup", "Pressia_Profile_Startup.json");
	auto app = Pressia::CreateApplication({ argc, argv });
	PS_PROFILE_END_SESSION();

	PS_PROFILE_BEGIN_SESSION("Runtime", "Pressia_Profile_Runtime.json");
	app->Run();
	PS_PROFILE_END_SESSION();

	PS_PROFILE_BEGIN_SESSION("Shutdown", "Pressia_Profile_Shutdown.json");
	delete app;
	PS_PROFILE_END_SESSION();
}

#endif // PS_PLATFORM_WINDOWS