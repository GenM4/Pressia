#include "pspch.h"
#include "Renderer.h"

#include "Pressia/Renderer/Renderer2D.h"

namespace Pressia {

	void Renderer::Init() {
		PS_PROFILE_RENDERER_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown() {
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		PS_PROFILE_RENDERER_FUNCTION();

		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera& camera) {
		PS_PROFILE_RENDERER_FUNCTION();

		//	Will call Renderer2D or Renderer3D
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform) {
		PS_PROFILE_RENDERER_FUNCTION();

		//	Will submit renderer commands to render queue
	}

}