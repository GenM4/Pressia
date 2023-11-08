#pragma once

#include "Pressia/Core/TimeStep.h"

namespace Pressia {

	class EngineInfoPanel {
	public:
		EngineInfoPanel() = default;

		void OnUpdate(Timestep ts);
		void OnImGuiRender();

		bool GetResetRenderStats() { return m_ResetRenderStats; }

	private:
		double m_TimePerFrame = 0.0f;
		bool m_ResetRenderStats = true;
	};
}

