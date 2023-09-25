#pragma once

#include "Pressia/Renderer/OrthographicCamera.h"
#include "Pressia/Core/TimeStep.h"

#include "Pressia/Events/ApplicationEvent.h"
#include "Pressia/Events/MouseEvent.h"

namespace Pressia {

	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f; //Default zoom of 1.0 (2.0 units)
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 70.0f;
	};
}


