#pragma once

#include "Pressia/Scene/SceneCamera.h"
#include "Pressia/Scene/ScriptableEntity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace Pressia {

	struct TagComponent {
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct TransformComponent {
		glm::vec3 Translation{ 0.0f };
		glm::vec3 Rotation{ 0.0f };
		glm::vec3 Scale{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) : Translation(translation) {}

		glm::mat4 GetTransform() const {
			auto transform = glm::mat4(1.0f);

			transform = glm::translate(transform, Translation)
				* glm::rotate(transform, Rotation.x, { 1, 0, 0 }) * glm::rotate(transform, Rotation.y, { 0, 1, 0 }) * glm::rotate(transform, Rotation.z, { 0, 0, 1 })
				* glm::scale(transform, Scale);

			return transform;
		}
	};

	struct SpriteRendererComponent {
		glm::vec4 Color{ 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
	};

	struct CameraComponent {
		SceneCamera Camera;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent {
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind() {
			InstantiateScript = []() {return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };

		}
	};
}