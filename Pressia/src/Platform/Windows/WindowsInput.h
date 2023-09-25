#pragma once

#include "Pressia/Core/Input.h"

namespace Pressia {

	class WindowsInput : public Input {
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual std::pair<float, float> GetMousePosImpl() override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}