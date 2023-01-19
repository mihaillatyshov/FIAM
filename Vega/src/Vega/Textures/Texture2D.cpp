#include "Texture2D.h"

#include "Vega/Core/Assert.h"
#include "Vega/Renderers/RendererAPI.h"
#include "Platform/OpenGL/Textures/OGLTexture2D.h"

#if defined(OPENGL2) || defined(OPENGL3)
#include "Platform/OpenGL/Textures/OGLTexture2D.h"
#endif
#ifdef OPENGL4
#include "Platform/OpenGL4/Textures/OGL4Texture2D.h"
#endif

namespace LM
{

	Ref<Texture2D> Texture2D::Create(std::string_view _FileName, Texture::MASK _Mask)
	{
		switch (RendererAPI::Get())
		{
			RENDERER_API_CASE_NONE(CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;);
			RENDERER_API_CASE_OGL2(return CreateRef<OGLTexture2D>(_FileName, _Mask));
			RENDERER_API_CASE_OGL3(return CreateRef<OGLTexture2D>(_FileName, _Mask));
			RENDERER_API_CASE_OGL4(return CreateRef<OGL4Texture2D>(_FileName, _Mask));
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Texture2D::~Texture2D()
	{

	}

	void Texture2D::Add(Ref<Texture> _Texture, std::string_view _Name, Ref<RendererCommand> _RendererCommand)
	{
		_RendererCommand->Add(StaticRefCast<Texture2D>(_Texture), _Name);
	}
	
	void Texture2D::Remove(std::string_view _Name, Ref<RendererCommand> _RendererCommand)
	{
		_RendererCommand->RemoveTexture2D(_Name);
	}

}