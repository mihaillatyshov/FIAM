#include "OGL4FrameBuffer.h"

#include <GL/glew.h>

#include "Vega/Core/Assert.h"

namespace LM
{

	OGL4FrameBuffer::OGL4FrameBuffer(const FrameBufferProps& _Props)
	{
		m_Width = _Props.Width;
		m_Height = _Props.Height;
		m_Samples = (uint32_t)_Props.MSAA;
		m_BufferMask = _Props.FBMask;

		for (const auto& Mask : _Props.FBColorMask)
		{
			m_ColorTextures.emplace_back(0u, 0u, Mask);
		}

		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_TextureMaxSize);

		OnAttach();
	}
	
	OGL4FrameBuffer::~OGL4FrameBuffer()
	{

	}

	void OGL4FrameBuffer::OnAttach()
	{
		glViewport(0, 0, m_Width, m_Height);
		glCreateFramebuffers(1, &m_BufferID);


		CreateTextures();

		CreateRenderBuffer();

		CheckStatus();
	}

	void OGL4FrameBuffer::OnDetach()
	{
		for (uint32_t i = 0; i < m_ColorTextures.size(); i++)
		{
			glDeleteTextures(1, &m_ColorTextures[i].Id);
		}

		if (HasRenderBuffer())
			glDeleteRenderbuffers(1, &m_RenderBufferId);

		glDeleteFramebuffers(1, &m_BufferID);
	}

	uint32_t OGL4FrameBuffer::GetTextureFormat()
	{
		return 0;
	}

	uint32_t OGL4FrameBuffer::GetTextureAttach()
	{
		return 0;
	}

	void OGL4FrameBuffer::CreateTextures()
	{
		std::vector<uint32_t> Attachments;
		for (uint32_t i = 0; i < m_ColorTextures.size(); i++)
		{
			uint32_t TextureType = m_Samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
			uint32_t TextureFormat = m_ColorTextures[i].Mask & FrameBufferColorMASK::HDR ? GL_RGBA16F : GL_RGBA8;
			glCreateTextures(TextureType, 1, &m_ColorTextures[i].Id);
			if (m_Samples)
			{
				glTextureStorage2DMultisample(m_ColorTextures[i].Id, m_Samples, TextureFormat, m_Width, m_Height, GL_TRUE);
			}
			else
			{
				glTextureStorage2D(m_ColorTextures[i].Id, 1, TextureFormat, m_Width, m_Height);

				glTextureParameteri(m_ColorTextures[i].Id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(m_ColorTextures[i].Id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTextureParameteri(m_ColorTextures[i].Id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(m_ColorTextures[i].Id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			glNamedFramebufferTexture(m_BufferID, GL_COLOR_ATTACHMENT0 + i, m_ColorTextures[i].Id, 0);
			Attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
		if (m_ColorTextures.size() > 1)
		{
			glNamedFramebufferDrawBuffers(m_BufferID, m_ColorTextures.size(), Attachments.data());
		}
	}

	uint32_t OGL4FrameBuffer::GetRenderBufferFormat()
	{
		if (m_BufferMask & FrameBufferMASK::DEPTH && m_BufferMask & FrameBufferMASK::STENCIL)
		{
			return GL_DEPTH24_STENCIL8;
		}
		else if (m_BufferMask & FrameBufferMASK::DEPTH)
		{
			return GL_DEPTH_COMPONENT32;
		}
		else // (m_BufferMask & FrameBufferMASK::STENCIL)
		{
			return GL_STENCIL_INDEX8;
		}
	}

	uint32_t OGL4FrameBuffer::GetRenderBufferAttach()
	{
		if (m_BufferMask & FrameBufferMASK::DEPTH && m_BufferMask & FrameBufferMASK::STENCIL)
		{
			return GL_DEPTH_STENCIL_ATTACHMENT;
		}
		else if (m_BufferMask & FrameBufferMASK::DEPTH)
		{
			return GL_DEPTH_ATTACHMENT;
		}
		else // (m_BufferMask & FrameBufferMASK::STENCIL)
		{
			return GL_STENCIL_ATTACHMENT;
		}
	}

	void OGL4FrameBuffer::CreateRenderBuffer()
	{
		if (!HasRenderBuffer())
			return;

		uint32_t RenderBufferFormat = GetRenderBufferFormat();
		uint32_t RenderBufferAttach = GetRenderBufferAttach();

		glCreateRenderbuffers(1, &m_RenderBufferId);
		if (m_Samples)
		{
			glNamedRenderbufferStorageMultisample(m_RenderBufferId, m_Samples, RenderBufferFormat, m_Width, m_Height);
		}
		else
		{
			glNamedRenderbufferStorage(m_RenderBufferId, RenderBufferFormat, m_Width, m_Height);
		}
		glNamedFramebufferRenderbuffer(m_BufferID, RenderBufferAttach, GL_RENDERBUFFER, m_RenderBufferId);
	}

	void OGL4FrameBuffer::CheckStatus()
	{
		//CORE_ASSERT(glCheckNamedFramebufferStatus(m_BufferID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FRAMEBUFFER: Framebuffer is not complete!");
		switch (glCheckNamedFramebufferStatus(m_BufferID, GL_FRAMEBUFFER))
		{
			case GL_FRAMEBUFFER_UNDEFINED: CORE_ASSERT(false, "FRAMEBUFFER: GL_FRAMEBUFFER_UNDEFINED"); break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: CORE_ASSERT(false, "FRAMEBUFFER: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"); break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: CORE_ASSERT(false, "FRAMEBUFFER: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"); break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: CORE_ASSERT(false, "FRAMEBUFFER: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"); break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: CORE_ASSERT(false, "FRAMEBUFFER: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"); break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: CORE_ASSERT(false, "FRAMEBUFFER: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"); break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: CORE_ASSERT(false, "FRAMEBUFFER: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"); break;
		}
	}

	void OGL4FrameBuffer::Bind() const
	{
		glViewport(0, 0, m_Width, m_Height);
		glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);
	}
	
	void OGL4FrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OGL4FrameBuffer::Clear(glm::vec4 _Color) const
	{
		float Depth = 1.0f;
		int Stencil = 0;
		for (int i = 0; i < m_ColorTextures.size(); i++)
		{
			glClearNamedFramebufferfv(m_BufferID, GL_COLOR, i, &_Color.x);
		}
		glClearNamedFramebufferfv(m_BufferID, GL_DEPTH, 0, &Depth);
		glClearNamedFramebufferiv(m_BufferID, GL_STENCIL, 0, &Stencil);
	}

	void OGL4FrameBuffer::Resize(uint32_t _Width, uint32_t _Height)
	{
		if (_Width == m_Width && _Height == m_Height)
			return;
		if (_Width == 0 || _Height == 0)
			return;
		if (_Width > m_TextureMaxSize || _Height > m_TextureMaxSize)
			return;

		m_Width = _Width;
		m_Height = _Height;

		for (uint32_t i = 0; i < m_ColorTextures.size(); i++)
		{
			glDeleteTextures(1, &m_ColorTextures[i].Id);
		}

		CreateTextures();

		if (!HasRenderBuffer())
			return;

		uint32_t RenderBufferFormat = GetRenderBufferFormat();
		if (m_Samples)
		{
			glNamedRenderbufferStorageMultisample(m_RenderBufferId, m_Samples, RenderBufferFormat, m_Width, m_Height);
		}
		else
		{
			glNamedRenderbufferStorage(m_RenderBufferId, RenderBufferFormat, m_Width, m_Height);
		}
	}

	void OGL4FrameBuffer::BindColorTexture(uint32_t _Id, uint32_t _SlotId)
	{
		glBindTextureUnit(_SlotId, m_ColorTextures[_Id].Id);
	}


}