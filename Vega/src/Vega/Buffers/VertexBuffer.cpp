#include "VertexBuffer.h"

#include "Vega/Core/Assert.h"
#include "Vega/Renderers/RendererAPI.h"

#if defined(OPENGL2) || defined(OPENGL3)
	#include "Platform/OpenGL/Buffers/OGLVertexBuffer.h"
#endif
#ifdef OPENGL4
	#include "Platform/OpenGL4/Buffers/OGL4VertexBuffer.h"
#endif

namespace LM
{

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t _Size)
	{
		switch (RendererAPI::Get())
		{
			RENDERER_API_CASE_NONE(CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;);
			RENDERER_API_CASE_OGL2(return CreateRef<OGLVertexBuffer>(_Size));
			RENDERER_API_CASE_OGL3(return CreateRef<OGLVertexBuffer>(_Size));
			RENDERER_API_CASE_OGL4(return CreateRef<OGL4VertexBuffer>(_Size));
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(const void* _Data, uint32_t _Size)
	{
		switch (RendererAPI::Get())
		{
			RENDERER_API_CASE_NONE(CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;);
			RENDERER_API_CASE_OGL2(return CreateRef<OGLVertexBuffer>(_Data, _Size));
			RENDERER_API_CASE_OGL3(return CreateRef<OGLVertexBuffer>(_Data, _Size));
			RENDERER_API_CASE_OGL4(return CreateRef<OGL4VertexBuffer>(_Data, _Size));
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


}