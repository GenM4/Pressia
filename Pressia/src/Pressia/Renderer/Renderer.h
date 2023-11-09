#pragma once
#include "Pressia/Renderer/RendererConfig.h"

#include "Pressia/Renderer/Camera.h"
#include "Pressia/Renderer/RenderCommand.h"
#include "Pressia/Renderer/Shader.h"

namespace Pressia {

	class Renderer {
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(Camera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:

	};


}



