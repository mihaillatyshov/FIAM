#include "Shader.h"

#include "Vega/Core/Assert.h"
#include "Vega/Renderers/RendererAPI.h"

#if defined(OPENGL)
#include "Platform/OpenGL/Shader/OGLShader.h"
#endif

namespace LM
{

//	Ref<Shader> Shader::Create(std::string_view _VertPath, std::string_view _FragPath)
//	{
//#ifdef OPENGL
//		return CreateRef<OGLShader>(_VertPath, _FragPath);
//#endif
//	}

	Ref<Shader> Shader::Create(const ShaderLayout& _Layout)
	{
		switch (RendererAPI::Get())
		{
			RENDERER_API_CASE_NONE(CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;);
			RENDERER_API_CASE_OGL2(return CreateRef<OGLShader>(_Layout));
			RENDERER_API_CASE_OGL3(return CreateRef<OGLShader>(_Layout));
			RENDERER_API_CASE_OGL4(return CreateRef<OGLShader>(_Layout));
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}