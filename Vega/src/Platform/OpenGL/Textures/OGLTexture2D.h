#pragma once

#include "Vega/Textures/Texture2D.h"
#include "Vega/Core/Assert.h"

namespace LM
{

	class OGLTexture2D : public Texture2D
	{
	public:
		OGLTexture2D(std::string_view _FileName, Texture::MASK _Mask);
		virtual ~OGLTexture2D();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void* GetTextureId() const override { return (void*)m_TextureID; }
		virtual void* GetHandle() const override { CORE_ASSERT(false, "OpengGL low 4 can't have handle!"); return 0; }

		virtual void Bind(uint32_t _SlotId) override;
		virtual void Unbind() override;
	protected:
		uint32_t m_TextureID;
	};

}