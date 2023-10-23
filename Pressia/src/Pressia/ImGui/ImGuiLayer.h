#pragma once

#include "Pressia/Core/Layer.h"
#include "Pressia/Events/KeyEvent.h"
#include "Pressia/Events/MouseEvent.h"
#include "Pressia/Events/ApplicationEvent.h"

namespace Pressia {

	class PRESSIA_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = false;
		float m_Time = 0.0f;
	};
}


