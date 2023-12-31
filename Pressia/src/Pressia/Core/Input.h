#pragma once

#include "Pressia/Core/Core.h"
#include "Pressia/Core/KeyCodes.h"
#include "Pressia/Core/MouseButtonCodes.h"

namespace Pressia {

	class PRESSIA_API Input {
	public:
		static bool IsKeyPressed(PSKeyCode keycode);

		static bool IsMouseButtonPressed(PSMouseCode button);
		static std::pair<float, float> GetMousePos();
		static float GetMouseX();
		static float GetMouseY();

	};
}