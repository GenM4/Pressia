#include <Pressia.h>
#include <Pressia/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

namespace Pressia {

	class PressiaChamber : public Application {
	public:
		PressiaChamber() : Application("Pressia Chamber") {
			PressiaChamber::GetWindow().SetVSync(true);
			PushLayer(new EditorLayer());
		}

		~PressiaChamber() {

		}

	};

	Application* CreateApplication() {
		return new PressiaChamber();
	}
}
