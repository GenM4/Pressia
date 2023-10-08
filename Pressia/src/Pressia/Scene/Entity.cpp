#include "pspch.h"
#include "Entity.h"

namespace Pressia {

	Entity::Entity(entt::entity handle, Scene* scene) : m_EntityHandle(handle), m_Scene(scene) {

	}
}