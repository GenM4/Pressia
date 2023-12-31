#include <Pressia.h>
#include <Pressia/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public Pressia::Layer {
public:
	ExampleLayer() : Layer("Example"), m_CameraController(1920.0f / 1080.0f, true) {
		m_VertexArray = Pressia::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.3f, 0.2f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
		};

		auto triangleVB = Pressia::VertexBuffer::Create(vertices, sizeof(vertices));

		Pressia::BufferLayout layout = {
			{ Pressia::ShaderDataType::Float3, "a_Position" },
			{ Pressia::ShaderDataType::Float4, "a_Color" }
		};

		triangleVB->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(triangleVB);

		uint32_t indices[3] = { 0, 1 ,2 };

		auto triangleIB = Pressia::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(triangleIB);

		m_SquareVA = Pressia::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		auto squareVB = Pressia::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		Pressia::BufferLayout squareVBLayout = {
			{ Pressia::ShaderDataType::Float3, "a_Position" },
			{ Pressia::ShaderDataType::Float2, "a_TexCoord" }
		};

		squareVB->SetLayout(squareVBLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1 ,2, 2, 3, 0 };

		auto squareIB = Pressia::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				//color = vec4(0.2, 0.3, 0.8, 1.0);
				//color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		std::string vertexSrc2 = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			
			uniform vec3 u_Color;

			void main() {
				color = vec4(u_Color, 1.0);
			}
		)";

		m_Shader = Pressia::Shader::Create("ColorVertexPos", vertexSrc, fragmentSrc);
		m_Shader2 = Pressia::Shader::Create("FlatColor", vertexSrc2, fragmentSrc2);
		auto textureShader = m_ShaderLibrary.Load("Assets/Shaders/Texture.glsl");

		m_Texture = Pressia::Texture2D::Create("Assets/Textures/Penguin.png");

		std::dynamic_pointer_cast<Pressia::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Pressia::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(Pressia::Timestep ts) override {
		//Update
		m_CameraController.OnUpdate(ts);

		// Render
		Pressia::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Pressia::RenderCommand::Clear();


		Pressia::Renderer::BeginScene(m_CameraController.GetCamera());


		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Pressia::OpenGLShader>(m_Shader2)->Bind();
		std::dynamic_pointer_cast<Pressia::OpenGLShader>(m_Shader2)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {

				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				Pressia::Renderer::Submit(m_Shader2, m_SquareVA, transform);
			}
		}
		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Pressia::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(0.75f)));

		//Triangle
		//Pressia::Renderer::Submit(m_Shader, m_VertexArray);

		Pressia::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Pressia::Event& e) override {
		m_CameraController.OnEvent(e);
	}

private:
	Pressia::ShaderLibrary m_ShaderLibrary;
	Pressia::Ref<Pressia::Shader> m_Shader;
	Pressia::Ref<Pressia::Shader> m_Shader2;
	Pressia::Ref<Pressia::VertexArray> m_VertexArray;
	Pressia::Ref<Pressia::VertexArray> m_SquareVA;
	Pressia::Ref<Pressia::Texture2D> m_Texture;

	Pressia::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Pressia::Application {
public:
	Sandbox() {
		// PushLayer(new ExampleLayer());
		Sandbox::GetWindow().SetVSync(true);
		PushLayer(new Sandbox2D());
	}

	~Sandbox() {

	}

};

Pressia::Application* Pressia::CreateApplication() {
	return new Sandbox();
}