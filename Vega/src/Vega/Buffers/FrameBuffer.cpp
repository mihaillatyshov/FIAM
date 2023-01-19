#include "FrameBuffer.h"

#include "Vega/Core/Assert.h"
#include "Vega/Renderers/RendererAPI.h"

#ifdef OPENGL3
#include "Platform/OpenGL3/Buffers/OGL3FrameBuffer.h"
#endif
#ifdef OPENGL4
#include "Platform/OpenGL4/Buffers/OGL4FrameBuffer.h"
#endif

namespace LM
{

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferProps& _Props)
	{
		switch (RendererAPI::Get())
		{
			RENDERER_API_CASE_NONE(CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;);
			RENDERER_API_CASE_OGL2(CORE_ASSERT(false, "OGL2 Does not support FBO!"); return nullptr;);
			RENDERER_API_CASE_OGL3(CORE_ASSERT(false, "OGL3 Need To Create FBO!"); return nullptr;);
			RENDERER_API_CASE_OGL4(return CreateRef<OGL4FrameBuffer>(_Props));
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}