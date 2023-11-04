#pragma once

#include "Pressia/Renderer/Framebuffer.h"

namespace Pressia {

	class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		virtual ~OpenGLFramebuffer();

		virtual void Invalidate() override;

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearColorAttachment(uint32_t attachmentIndex, int clearValue) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { PS_CORE_ASSERT(index < m_ColorAttachments.size(), "Indexed framebuffer color attachment doesn't exist"); return m_ColorAttachments[index]; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		//	Attachment Formats
		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		FramebufferTextureSpecification m_DepthAttachmentSpec;

		//	Attachments
		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}
