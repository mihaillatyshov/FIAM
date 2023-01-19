#include "RendererCommand.h"

#include "Vega/Textures/Texture2D.h"
#include "Vega/Textures/TextureManager.h"

namespace LM
{

	RendererCommand::RendererCommand()
	{
		m_TextureManager = CreateRef<TextureManager>();
	}

	void RendererCommand::Add(Ref<Texture2D> _Texture, std::string_view _Name)
	{
		m_AddTexture2D.Push(_Texture);
		m_TextureManager->Add(_Texture, _Name);
	}

	void RendererCommand::RemoveTexture2D(std::string_view _Name)
	{
		m_TextureManager->Remove(_Name);
	}

}