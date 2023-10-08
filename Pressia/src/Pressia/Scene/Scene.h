#pragma once

#include "Pressia/Core/TimeStep.h"

#include <entt.hpp>

namespace Pressia {

	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;

		friend class Entity;
	};

}



