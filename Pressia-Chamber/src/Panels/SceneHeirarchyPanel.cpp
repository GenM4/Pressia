#include "SceneHeirarchyPanel.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

namespace Pressia {

	SceneHeirarchyPanel::SceneHeirarchyPanel(const Ref<Scene>& context) {
		SetContext(context);
	}

	void SceneHeirarchyPanel::SetContext(const Ref<Scene>& context) {
		m_Context = context;
	}

	void SceneHeirarchyPanel::OnImGuiRender() {
		ImGui::Begin("Scene Heirarchy");

		m_Context->m_Registry.each(
			[&](auto entityID) {
				Entity entity{ entityID, m_Context.get() };
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		ImGui::End();


		ImGui::Begin("Properties");

		if (m_SelectionContext) {
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}

	void SceneHeirarchyPanel::DrawEntityNode(Entity entity) {
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked()) {
			m_SelectionContext = entity;
		}

		if (opened) {
			ImGui::TreePop();
		}
	}

	void SceneHeirarchyPanel::DrawComponents(Entity selectionContext) {
		if (selectionContext.HasComponent<TagComponent>()) {
			auto& tag = selectionContext.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
		}

		if (selectionContext.HasComponent<TransformComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
				auto& transform = selectionContext.GetComponent<TransformComponent>().Transform;

				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.5f);

				ImGui::TreePop();
			}
		}

		if (selectionContext.HasComponent<CameraComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
				auto& camera = selectionContext.GetComponent<CameraComponent>().Camera;
				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
					for (int i = 0; i < 2; i++) {
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];

						if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected) {
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
					float orthoSize = camera.GetOrthographicSize();
					float orthoNear = camera.GetOrthographicNearClip();
					float orthoFar = camera.GetOrthographicFarClip();

					if (ImGui::DragFloat("Size", &orthoSize)) {
						camera.SetOrthographicSize(orthoSize);
					}
					if (ImGui::DragFloat("Near", &orthoNear)) {
						camera.SetOrthographicNearClip(orthoNear);
					}
					if (ImGui::DragFloat("Far", &orthoFar)) {
						camera.SetOrthographicFarClip(orthoFar);
					}
				}
				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
					float perspectiveFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
					float perspectiveNear = camera.GetPerspectiveNearClip();
					float perspectiveFar = camera.GetPerspectiveFarClip();

					if (ImGui::DragFloat("FOV", &perspectiveFOV)) {
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveFOV));
					}
					if (ImGui::DragFloat("Near", &perspectiveNear)) {
						camera.SetPerspectiveNearClip(perspectiveNear);
					}
					if (ImGui::DragFloat("Far", &perspectiveFar)) {
						camera.SetPerspectiveFarClip(perspectiveFar);
					}
				}
			}
			ImGui::TreePop();
		}
	}

}