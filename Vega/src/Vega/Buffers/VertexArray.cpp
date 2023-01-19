#include "VertexArray.h"

#include "Vega/Core/Assert.h"
#include "Vega/Renderers/RendererAPI.h"

#ifdef OPENGL2
	#include "Platform/OpenGL2/Buffers/OGL2VertexArray.h"
#endif
#ifdef OPENGL3
	#include "Platform/OpenGL3/Buffers/OGL3VertexArray.h"
#endif
#ifdef OPENGL4
	#include "Platform/OpenGL4/Buffers/OGL4VertexArray.h"
#endif

namespace LM
{

	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::Get())
		{
			RENDERER_API_CASE_NONE(CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;);
			RENDERER_API_CASE_OGL2(return CreateRef<OGL2VertexArray>());
			RENDERER_API_CASE_OGL3(return CreateRef<OGL3VertexArray>());
			RENDERER_API_CASE_OGL4(return CreateRef<OGL4VertexArray>());
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}