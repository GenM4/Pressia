#include <Pressia.h>

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Pressia::Layer {
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f, 0.0f, 0.0f) {
		m_VertexArray.reset(Pressia::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.3f, 0.2f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
		};

		std::shared_ptr<Pressia::VertexBuffer> triangleVB;
		triangleVB.reset(Pressia::VertexBuffer::Create(vertices, sizeof(vertices)));

		Pressia::BufferLayout layout = {
			{ Pressia::ShaderDataType::Float3, "a_Position" },
			{ Pressia::ShaderDataType::Float4, "a_Color" }
		};

		triangleVB->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(triangleVB);

		uint32_t indices[3] = { 0, 1 ,2 };
		std::shared_ptr<Pressia::IndexBuffer> triangleIB;
		triangleIB.reset(Pressia::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(triangleIB);

		m_SquareVA.reset(Pressia::VertexArray::Create());

		float squareVertices[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<Pressia::VertexBuffer> squareVB;
		squareVB.reset(Pressia::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Pressia::BufferLayout squareVBLayout = {
			{ Pressia::ShaderDataType::Float3, "a_Position" }
		};

		squareVB->SetLayout(squareVBLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1 ,2, 2, 3, 0 };
		std::shared_ptr<Pressia::IndexBuffer> squareIB;
		squareIB.reset(Pressia::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

			void main() {
				color = vec4(0.2, 0.3, 0.8, 1.0);
				//color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		m_Shader.reset(Pressia::Shader::Create(vertexSrc, fragmentSrc));
		m_Shader2.reset(Pressia::Shader::Create(vertexSrc2, fragmentSrc2));
	}

	void OnUpdate(Pressia::Timestep ts) override {

		float time = ts;
		// Camera
		if (Pressia::Input::IsKeyPressed(PS_KEY_A))
			m_CameraPosition.x += m_CameraSpeed * ts;

		if (Pressia::Input::IsKeyPressed(PS_KEY_D))
			m_CameraPosition.x -= m_CameraSpeed * ts;

		if (Pressia::Input::IsKeyPressed(PS_KEY_S))
			m_CameraPosition.y += m_CameraSpeed * ts;

		if (Pressia::Input::IsKeyPressed(PS_KEY_W))
			m_CameraPosition.y -= m_CameraSpeed * ts;

		if (Pressia::Input::IsKeyPressed(PS_KEY_Q))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		else if (Pressia::Input::IsKeyPressed(PS_KEY_E))
			m_CameraRotation += m_CameraRotationSpeed * ts;




		Pressia::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Pressia::RenderCommand::Clear();
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);


		Pressia::Renderer::BeginScene(m_Camera);
		{

			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

			for (int y = 0; y < 20; y++) {
				for (int x = 0; x < 20; x++) {

					glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
					Pressia::Renderer::Submit(m_Shader2, m_SquareVA, transform);
				}
			}

			//Pressia::Renderer::Submit(m_Shader, m_VertexArray);
		}
		Pressia::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {

	}

	void OnEvent(Pressia::Event& event) override {

	}

private:
	std::shared_ptr<Pressia::Shader> m_Shader;
	std::shared_ptr<Pressia::Shader> m_Shader2;
	std::shared_ptr<Pressia::VertexArray> m_VertexArray;
	std::shared_ptr<Pressia::VertexArray> m_SquareVA;

	Pressia::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 1.0f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 70.0f;
};

class Sandbox : public Pressia::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());

	}

	~Sandbox() {

	}

};

Pressia::Application* Pressia::CreateApplication() {
	return new Sandbox();
}