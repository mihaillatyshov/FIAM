#include "IndexBuffer.h"

#include "Vega/Core/Assert.h"
#include "Vega/Renderers/RendererAPI.h"

#if defined(OPENGL2) || defined(OPENGL3)
	#include "Platform/OpenGL/Buffers/OGLIndexBuffer.h"
#endif
#ifdef OPENGL4
	#include "Platform/OpenGL4/Buffers/OGL4IndexBuffer.h"
#endif

namespace LM
{

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t _Count)
	{
		switch (RendererAPI::Get())
		{
			RENDERER_API_CASE_NONE(CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;);
			RENDERER_API_CASE_OGL2(return CreateRef<OGLIndexBuffer>(_Count));
			RENDERER_API_CASE_OGL3(return CreateRef<OGLIndexBuffer>(_Count));
			RENDERER_API_CASE_OGL4(return CreateRef<OGL4IndexBuffer>(_Count));
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* _Data, uint32_t _Count)
	{
		switch (RendererAPI::Get())
		{
			RENDERER_API_CASE_NONE(CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;);
			RENDERER_API_CASE_OGL2(return CreateRef<OGLIndexBuffer>(_Data, _Count));
			RENDERER_API_CASE_OGL3(return CreateRef<OGLIndexBuffer>(_Data, _Count));
			RENDERER_API_CASE_OGL4(return CreateRef<OGL4IndexBuffer>(_Data, _Count));
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}