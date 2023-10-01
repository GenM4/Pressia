#pragma once

#include "Pressia/Core/Layer.h"
#include "Pressia/Events/KeyEvent.h"
#include "Pressia/Events/MouseEvent.h"
#include "Pressia/Events/ApplicationEvent.h"

namespace Pressia {

	class PRESSIA_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}


