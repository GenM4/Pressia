#pragma once

#include "Pressia/Core/Core.h"
#include "Pressia/Renderer/RendererConfig.h"

namespace Pressia {

	enum class FramebufferTextureFormat {
		None = 0,

		//	Color
		RED_INTEGER,
		RGBA8,

		//	Depth
		DEPTH24STENCIL8,

		//	Default
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification {
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format) : TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		//	TODO:	Filtering/wrapping
	};

	struct FramebufferAttachmentSpecification {
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list< FramebufferTextureSpecification> attachments) : Attachments(attachments) {}

		std::vector< FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification {
		uint32_t Width = 0, Height = 0;

		FramebufferAttachmentSpecification Attachments;

		uint32_t Samples = 1;

		bool SwapChainTarget = false;	//true = render to screen, false = render to framebuffer instead
	};

	class Framebuffer {
	public:
		virtual ~Framebuffer() = default;

		virtual void Invalidate() = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearColorAttachment(uint32_t attachmentIndex, int clearValue) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& specification);
	};
}
