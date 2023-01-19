#pragma once

#include <string>
#include <mutex>

#include "Vega/Core/Base.h"
#include "Vega/Threads/QueueMT.h"

namespace LM
{

	class Renderer;
	class Texture2D;
	class TextureManager;

	class RendererCommand
	{
	public:
		RendererCommand();
		virtual ~RendererCommand() = default;
		virtual void UpdateRendererData(Ref<Renderer> _Renderer) = 0;
		
		virtual void Clear() = 0;
		virtual void Add(Ref<Texture2D> _Texture, std::string_view _Name);
		virtual void RemoveTexture2D(std::string_view _Name);
	protected:
		QueueMT<Ref<Texture2D>> m_AddTexture2D;
		QueueMT<std::string> m_RemoveTexture2D;
		Ref<TextureManager> m_TextureManager;
	};

}