#include "SceneHeirarchyPanel.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

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

		//	Right click menu (empty space)
		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();


		ImGui::Begin("Properties");

		if (m_SelectionContext) {
			DrawComponents(m_SelectionContext);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("Add Component");

			if (ImGui::BeginPopup("Add Component")) {
				if (ImGui::MenuItem("Camera") && !m_SelectionContext.HasComponent<CameraComponent>()) {
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Renderer") && !m_SelectionContext.HasComponent<SpriteRendererComponent>()) {
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
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

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem(0, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened) {
			ImGui::TreePop();
		}

		if (entityDeleted) {
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.15f, 0.8f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.9f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.15f, 0.8f, 0.1f, 1.0f });

		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.15f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.15f, 0.8f, 1.0f });

		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
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

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (selectionContext.HasComponent<TransformComponent>()) {
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("...", ImVec2{ 20, 20 }))
				ImGui::OpenPopup("Component Settings");

			if (ImGui::BeginPopup("Component Settings")) {
				//	TODO: Add copy component functionality

				ImGui::EndPopup();
			}

			if (open) {
				auto& tc = selectionContext.GetComponent<TransformComponent>();

				DrawVec3Control("Position", tc.Translation);
				auto rotation = glm::degrees(tc.Rotation);
				DrawVec3Control("Rotation", rotation);
				tc.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", tc.Scale, 1.0f);

				ImGui::TreePop();
			}
		}

		if (selectionContext.HasComponent<SpriteRendererComponent>()) {
			bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite Renderer");
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("...", ImVec2{ 20, 20 }))
				ImGui::OpenPopup("Component Settings");

			bool removeComponent = false;
			if (ImGui::BeginPopup("Component Settings")) {
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				//	TODO: Add copy component functionality

				ImGui::EndPopup();
			}

			if (open) {
				auto& src = selectionContext.GetComponent<SpriteRendererComponent>();

				ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));

				ImGui::TreePop();
			}

			if (removeComponent)
				selectionContext.RemoveComponent<SpriteRendererComponent>();
		}

		if (selectionContext.HasComponent<CameraComponent>()) {
			bool open = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera");
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("...", ImVec2{ 20, 20 }))
				ImGui::OpenPopup("Component Settings");

			bool removeComponent = false;
			if (ImGui::BeginPopup("Component Settings")) {
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				//	TODO: Add copy component functionality

				ImGui::EndPopup();
			}

			if (open) {
				auto& camera = selectionContext.GetComponent<CameraComponent>().Camera;

				//	Camera Selector
				bool selectedCamera = m_Context->m_CurrentCamera == &camera;
				if (ImGui::Checkbox("Selected", &selectedCamera)) {
					m_Context->SetCamera(camera);
				}

				//	Projection Selector
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

				//	Orthographic Projection Properties
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

				//	Perspective Projection Properties
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

			if (removeComponent)
				selectionContext.RemoveComponent<CameraComponent>();
		}
	}

}