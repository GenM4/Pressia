#include "pspch.h"
#include "Pressia/Core/Input.h"

#include "Pressia/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Pressia {

	bool Input::IsKeyPressed(PSKeyCode keycode) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, (int)keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	std::pair<float, float> Input::GetMousePos() {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	bool Input::IsMouseButtonPressed(PSMouseCode button) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, (int)button);

		return state == GLFW_PRESS;
	}

	float Input::GetMouseX() {
		auto [x, y] = GetMousePos();
		return x;
	}

	float Input::GetMouseY() {
		auto [x, y] = GetMousePos();
		return y;
	}
}