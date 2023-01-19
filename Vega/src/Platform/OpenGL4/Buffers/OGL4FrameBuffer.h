#pragma once

#include "Vega/Buffers/FrameBuffer.h"

namespace LM
{

	class OGL4FrameBuffer : public FrameBuffer
	{
	public:
		OGL4FrameBuffer(const FrameBufferProps& _Props);
		~OGL4FrameBuffer();

		void OnAttach();
		void OnDetach();

		virtual void Bind()	const override;
		virtual void Unbind() const override;

		virtual void Clear(glm::vec4 _Color) const override;

		virtual void* GetId() const { return (void*)m_BufferID; }
		virtual uint32_t GetTextureCount() const override { return m_ColorTextures.size(); }
		virtual void* GetTextureId(uint32_t _Id = 0) const override { return (void*)m_ColorTextures[_Id].Id; }
		virtual void* GetTextureHandle(uint32_t _Id = 0) const override { return (void*)m_ColorTextures[_Id].Handle; }
		bool HasRenderBuffer() const { return m_BufferMask & FrameBufferMASK::DEPTH || m_BufferMask & FrameBufferMASK::STENCIL; }

		virtual uint32_t GetWidth() const { return m_Width; }
		virtual uint32_t GetHeight() const { return m_Height; }

		virtual void Resize(uint32_t _Width, uint32_t _Height) override;

		virtual void BindColorTexture(uint32_t _Id, uint32_t _SlotId) override;
	protected:
		uint32_t GetTextureFormat();
		uint32_t GetTextureAttach();
		void CreateTextures();
		uint32_t GetRenderBufferFormat();
		uint32_t GetRenderBufferAttach();
		void CreateRenderBuffer();
		void CheckStatus();
	protected:
		struct ColorTexture
		{
			uint32_t Id;
			uint64_t Handle;
			uint32_t Mask;
		};
	protected:
		uint32_t m_BufferID;
		uint32_t m_RenderBufferId;
	
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_Samples;

		int m_TextureMaxSize;

		std::vector<ColorTexture> m_ColorTextures;
		uint32_t m_BufferMask;
	};

}