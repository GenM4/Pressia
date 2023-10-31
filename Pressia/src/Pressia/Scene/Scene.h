#pragma once

#include "Pressia/Core/TimeStep.h"
#include "Pressia/Renderer/Camera.h"

#include <entt.hpp>

namespace Pressia {

	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnViewportResize(uint32_t width, uint32_t height);
		void OnUpdate(Timestep ts);

		void SetCamera(Camera& camera);
		void SetCamera(Entity& cameraEntity);

		Ref<Entity> GetCamera() { return m_CurrentCameraEntity; }

		void CreateDefaultCamera();
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		Ref<Entity> m_CurrentCameraEntity = nullptr;
		Camera* m_CurrentCamera = nullptr;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHeirarchyPanel;
	};

}



