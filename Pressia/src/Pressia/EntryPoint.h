#pragma once

#ifdef PS_PLATFORM_WINDOWS

extern Pressia::Application* Pressia::CreateApplication();

int main(int argc, char** argv) {

	Pressia::Log::Init();
	PS_CORE_WARN("Initialized Logger!");
	int a = 5;
	PS_INFO("Hello! Var={0}", a);

	auto app = Pressia::CreateApplication();
	app->Run();
	delete app;
}

#endif // PS_PLATFORM_WINDOWS