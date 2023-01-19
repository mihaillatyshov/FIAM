#pragma once

#include "Vega/Textures/Texture2D.h"

namespace LM
{

	class OGL4Texture2D : public Texture2D
	{
	public:
		OGL4Texture2D(std::string_view _FileName, Texture::MASK _Mask);
		virtual ~OGL4Texture2D();

		virtual void Bind(uint32_t _SlotId) override;
		virtual void Unbind() override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void* GetTextureId() const override { return (void*)m_TextureID; }
		virtual void* GetHandle() const override { return (void*)m_TextureHandle; }
	protected:
		void LoadOnError();
	protected:
		std::string m_FileName;
		MASK m_Mask;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_TextureID = 0;
		uint64_t m_TextureHandle = 0;
	};

}