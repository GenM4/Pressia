#include "pspch.h"
#include "SceneSerializer.h"

#include "Pressia/Scene/Entity.h"
#include "Pressia/Scene/Components.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

namespace Pressia {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	Pressia::SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : m_Scene(scene) {

	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity) {
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "43434342222444";	//TODO: UUID system for entities

		if (entity.HasComponent<TagComponent>()) {
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>()) {
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>()) {
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cc = entity.GetComponent<CameraComponent>();
			auto& camera = cc.Camera;
			out << YAML::Key << "Camera";

			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap;

			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>()) {
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& src = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << src.Color;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void Pressia::SceneSerializer::SerializeText(const std::string& filepath) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled Scene";	//TODO: add scene naming
		out << YAML::Key << "ActiveCamera" << YAML::Value << m_Scene->m_CurrentCameraEntity->GetComponent<TagComponent>().Tag;	//TODO: use uuid instead	
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID) {
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void Pressia::SceneSerializer::SerializeRuntime(const std::string& filepath) {
		PS_CORE_ASSERT(false, "Not implemented");
	}

	bool Pressia::SceneSerializer::DeserializeText(const std::string& filepath) {
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		PS_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		std::string activeCamTag = data["ActiveCamera"].as<std::string>();
		bool isActiveCamera = false;

		auto entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {
				uint64_t uuid = entity["Entity"].as<uint64_t>();	//TODO: entity UUID

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent) {
					name = tagComponent["Tag"].as<std::string>();
					if (name == activeCamTag) {
						isActiveCamera = true;
					}
				}

				PS_CORE_TRACE("Deserialized entity with ID: {0},/tName: {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name);	// TODO: Pass UUID in here

				auto tcIn = entity["TransformComponent"];
				if (tcIn) {
					auto& tcTarget = deserializedEntity.GetComponent<TransformComponent>();
					tcTarget.Translation = tcIn["Translation"].as<glm::vec3>();
					tcTarget.Rotation = tcIn["Rotation"].as<glm::vec3>();
					tcTarget.Scale = tcIn["Scale"].as<glm::vec3>();
				}

				auto ccIn = entity["CameraComponent"];
				if (ccIn) {
					auto& ccTarget = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = ccIn["Camera"];
					ccTarget.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					ccTarget.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					ccTarget.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					ccTarget.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					ccTarget.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					ccTarget.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					ccTarget.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					ccTarget.FixedAspectRatio = ccIn["FixedAspectRatio"].as<bool>();

					if (isActiveCamera)
						m_Scene->SetCamera(deserializedEntity);
				}

				auto srcIn = entity["SpriteRendererComponent"];
				if (srcIn) {
					auto& srcTarget = deserializedEntity.AddComponent<SpriteRendererComponent>();
					srcTarget.Color = srcIn["Color"].as<glm::vec4>();
				}
			}
		}
	}

	bool Pressia::SceneSerializer::DeserializeRuntime(const std::string& filepath) {
		PS_CORE_ASSERT(false, "Not implemented");
		return false;
	}

}

namespace YAML {
	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}