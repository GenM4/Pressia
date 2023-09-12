#pragma once

#include "pspch.h"

#include "Pressia/Core.h"
#include "Pressia/Events/Event.h"

namespace Pressia {

	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Pressia Engine",
			unsigned int width = 1920,
			unsigned int height = 1080)
			: Title(title), Width(width), Height(height) {

		}
	};


	// Interface for Desktop application window
	class PRESSIA_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//Window Attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}