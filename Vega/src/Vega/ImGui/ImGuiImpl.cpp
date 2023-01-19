#include "ImGuiImpl.h"

#include "Vega/Core/Assert.h"
#include "Vega/Renderers/RendererAPI.h"

#ifdef OPENGL2
#include "Platform/OpenGL2/ImGui/OGL2ImGuiImpl.h"
#endif
#ifdef OPENGL3
#include "Platform/OpenGL3/ImGui/OGL3ImGuiImpl.h"
#endif
#ifdef OPENGL4
#include "Platform/OpenGL4/ImGui/OGL4ImGuiImpl.h"
#endif

namespace LM
{

	Ref<ImGuiImpl> ImGuiImpl::Create()
	{
		switch (RendererAPI::Get())
		{
			RENDERER_API_CASE_NONE(CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;);
			RENDERER_API_CASE_OGL2(CORE_ASSERT(false, "OpenGL2 is not support ImGui!"); return nullptr;);
			RENDERER_API_CASE_OGL3(return CreateRef<OGL3ImGuiImpl>());
			RENDERER_API_CASE_OGL4(return CreateRef<OGL4ImGuiImpl>());
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}