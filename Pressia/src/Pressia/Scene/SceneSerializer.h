#pragma once

#include "Pressia/Scene/Scene.h"

namespace Pressia {
	class SceneSerializer {
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void SerializeText(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool DeserializeText(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		Ref<Scene> m_Scene;
	};
}


