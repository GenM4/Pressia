#pragma once

#include "Pressia.h"

namespace Pressia {

	class SceneHeirarchyPanel {
	public:
		SceneHeirarchyPanel() = default;
		SceneHeirarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity selectionContext);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}


