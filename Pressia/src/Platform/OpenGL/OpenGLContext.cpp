#include "pspch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Pressia {


	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
		PS_CORE_ASSERT(windowHandle, "Window handle is null!");
	}
	void OpenGLContext::Init() {
		PS_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PS_CORE_ASSERT(status, "Failed to initialize glad!");

		PS_CORE_INFO("OpenGL Info:");
		PS_CORE_INFO("		Vendor: {0}", glGetString(GL_VENDOR));
		PS_CORE_INFO("		Renderer: {0}", glGetString(GL_RENDERER));
		PS_CORE_INFO("		Version: {0}", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers() {
		PS_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}