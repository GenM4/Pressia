#pragma once

#include "Pressia.h"

namespace Pressia {

	class SceneHeirarchyPanel {
	public:
		SceneHeirarchyPanel() = default;
		SceneHeirarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

		Entity GetSelectedEntity() { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; }

		void CopySelectedEntity();
		void PasteCopiedEntity();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity selectionContext);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
		Entity m_LastCopiedEntity;
	};
}


