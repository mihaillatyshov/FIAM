#include "Renderer3D.h"

#include "Vega/Core/Assert.h"
#include "Vega/Renderers/RendererAPI.h"

#ifdef OPENGL2
	#include "Platform/OpenGL2/Renderers/OGL2Renderer3D.h"
#endif
#ifdef OPENGL3
	#include "Platform/OpenGL3/Renderers/Renderer3D/OGL3Renderer3D.h"
#endif
#ifdef OPENGL4
	#include "Platform/OpenGL4/Renderers/Renderer3D/OGL4Renderer3D.h"
#endif

namespace LM
{

	Ref<Renderer3D> Renderer3D::Create()
	{
		switch (RendererAPI::Get())
		{
			RENDERER_API_CASE_NONE(CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;);
			RENDERER_API_CASE_OGL2(return CreateRef<OGL2Renderer3D>());
			RENDERER_API_CASE_OGL3(return CreateRef<OGL3Renderer3D>());
			RENDERER_API_CASE_OGL4(return CreateRef<OGL4Renderer3D>());
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}