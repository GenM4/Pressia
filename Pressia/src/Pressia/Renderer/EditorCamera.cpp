#include "pspch.h"
#include "EditorCamera.h"

#include "Pressia/Core/Input.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Pressia {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip) : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip) {
		UpdateView();
	}

	EditorCamera::EditorCamera(EditorCamera& other) : m_FOV(other.m_FOV), m_AspectRatio(other.m_AspectRatio), m_NearClip(other.m_NearClip), m_FarClip(other.m_FarClip) {
		UpdateView();
	}

	EditorCamera EditorCamera::operator=(EditorCamera& other) {

		return EditorCamera(other);
	}

	void EditorCamera::OnUpdate(Timestep ts) {
		if (Input::IsKeyPressed(PSKeyCode::LEFT_CONTROL)) {
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_InitialMousePos) * 0.005f;
			m_InitialMousePos = mouse;

			if (Input::IsMouseButtonPressed(PSMouseCode::PS_MOUSE_BUTTON_LEFT))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(PSMouseCode::PS_MOUSE_BUTTON_RIGHT))
				MouseZoom(delta.y);
			else if (Input::IsMouseButtonPressed(PSMouseCode::PS_MOUSE_BUTTON_MIDDLE))
				MousePan(delta);
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(PS_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	glm::vec3 EditorCamera::CalculatePosition() const {
		return m_FocalPoint - GetLForwardDirection() * m_Distance;
	}

	void EditorCamera::UpdateProjection() {
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView() {
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e) {
		float delta = e.GetYOffset() * 0.1f;

		MouseZoom(delta);
		UpdateView();

		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta) {
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta) {
		float yawDirection = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawDirection * delta.x * m_RotationSpeed;
		m_Pitch += delta.y * m_RotationSpeed;
	}

	void EditorCamera::MouseZoom(float delta) {
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f) {
			m_FocalPoint += GetLForwardDirection();
			m_Distance = 1.0f;
		}
	}

	std::pair<float, float> EditorCamera::PanSpeed() const {
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f);
		float xFactor = 0.05f * (x * x) - 0.2f * x + 0.4f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f);
		float yFactor = 0.05f * (y * y) - 0.2f * y + 0.4f;

		return std::pair<float, float>(xFactor, yFactor);
	}

	float EditorCamera::ZoomSpeed() const {
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.2f);

		float speed = distance * distance;
		speed = std::min(speed, 100.0f);

		return speed;
	}

	glm::vec3 EditorCamera::GetUpDirection() const {

		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const {

		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetLForwardDirection() const {

		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::GetOrientation() const {

		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}
}