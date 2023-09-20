#include "pspch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Pressia {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}