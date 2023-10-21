#pragma once

#include <glm/glm.hpp>

namespace Pressia {

	class Camera {
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) : m_Projection(projection) {}
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }

		bool operator==(Camera other) { return this == &other; }
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}