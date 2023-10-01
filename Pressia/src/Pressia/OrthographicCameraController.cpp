#include "pspch.h"
#include "OrthographicCameraController.h"

#include "Pressia/Core/Input.h"

namespace Pressia {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation) {

	}

	void OrthographicCameraController::OnUpdate(Timestep ts) {
		PS_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(PS_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;

		else if (Input::IsKeyPressed(PS_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(PS_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;

		else if (Input::IsKeyPressed(PS_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

		if (m_Rotation) {
			if (Input::IsKeyPressed(PS_KEY_Q))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			else if (Input::IsKeyPressed(PS_KEY_E))
				m_CameraRotation += m_CameraRotationSpeed * ts;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		PS_PROFILE_RENDERER_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(PS_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(PS_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		PS_PROFILE_RENDERER_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
		PS_PROFILE_RENDERER_FUNCTION();

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}
}