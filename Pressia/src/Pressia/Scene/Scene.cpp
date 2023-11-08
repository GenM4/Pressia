#include "pspch.h"
#include "Scene.h"

#include "Pressia/Renderer/Renderer2D.h"
#include "Pressia/Scene/Entity.h"
#include "Pressia/Scene/Components.h"

namespace Pressia {

	Scene::Scene() {

	}

	Scene::~Scene() {

	}

	Entity Scene::CreateEntity(const std::string& name) {
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Unnamed Entity" : name;
		return entity;
	}

	Entity Scene::DuplicateEntity(Entity srcEntity) {
		std::string name = srcEntity.GetComponent<TagComponent>().Tag;

		auto destinationEntity = CreateEntity();
		destinationEntity.RemoveComponent<TagComponent>();
		destinationEntity.RemoveComponent<TransformComponent>();

		for (auto&& currentComponent : m_Registry.storage()) {
			if (auto& storage = currentComponent.second; storage.contains(srcEntity))
				storage.push(destinationEntity, storage.value(srcEntity));
		}
		return destinationEntity;
	}

	void Scene::DestroyEntity(Entity entity) {
		m_Registry.destroy(entity);
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	void Scene::OnUpdateRuntime(Timestep ts) {

		// Update Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
				// Move to OnScenePlay once implemented
				if (!nsc.Instance) {
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
				});
		}

		// Render 2D

		glm::mat4 cameraTransform;

		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view) {
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (&camera.Camera == m_CurrentCamera) {
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		Renderer2D::BeginScene(m_CurrentCamera->GetProjection(), cameraTransform);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) {
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera) {
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) {
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::EndScene();
	}

	void Scene::SetCamera(Camera& camera) {
		m_CurrentCamera = &camera;
	}

	void Scene::SetCamera(Entity& cameraEntity) {
		m_CurrentCameraEntity = CreateRef<Entity>(cameraEntity);
		auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;

		SetCamera(camera);
	}

	void Scene::CreateDefaultCamera() {
		auto defaultCam = CreateEntity("Default Camera");
		defaultCam.AddComponent<CameraComponent>();
		SetCamera(defaultCam);
	}

}