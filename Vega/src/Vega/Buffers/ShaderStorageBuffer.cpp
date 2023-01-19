#include "ShaderStorageBuffer.h"


#include "Vega/Core/Assert.h"
#include "Vega/Renderers/RendererAPI.h"

#if defined(OPENGL2) || defined(OPENGL3)
//#include "Platform/OpenGL/Buffers/OGLShaderStorageBuffer.h"
#endif
#ifdef OPENGL4
#include "Platform/OpenGL4/Buffers/OGL4ShaderStorageBuffer.h"
#endif

namespace LM
{

	Ref<ShaderStorageBuffer> ShaderStorageBuffer::Create(uint32_t _Size)
	{
		switch (RendererAPI::Get())
		{
			RENDERER_API_CASE_NONE(CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;);
			RENDERER_API_CASE_OGL2(CORE_ASSERT(false, "OGL2 Does not support SSBO!"); return nullptr;);
			RENDERER_API_CASE_OGL3(CORE_ASSERT(false, "OGL3 Does not support SSBO!"); return nullptr;);
			RENDERER_API_CASE_OGL4(return CreateRef<OGL4ShaderStorageBuffer>(_Size));
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<ShaderStorageBuffer> ShaderStorageBuffer::Create(const void* _Data, uint32_t _Size)
	{
		switch (RendererAPI::Get())
		{
			RENDERER_API_CASE_NONE(CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;);
			RENDERER_API_CASE_OGL2(CORE_ASSERT(false, "OGL2 Does not support SSBO!"); return nullptr;);
			RENDERER_API_CASE_OGL3(CORE_ASSERT(false, "OGL3 Does not support SSBO!"); return nullptr;);
			RENDERER_API_CASE_OGL4(return CreateRef<OGL4ShaderStorageBuffer>(_Data, _Size));
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


}