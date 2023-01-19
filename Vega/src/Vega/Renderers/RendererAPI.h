#pragma once

#define RENDERER_API_CASE_NONE(x) case LM::RendererAPI::API::None: x

#ifdef OPENGL2
	#define RENDERER_API_CASE_OGL2(x) case LM::RendererAPI::API::OpenGL2: x
#else
	#define RENDERER_API_CASE_OGL2(x) 
#endif

#ifdef OPENGL3
	#define RENDERER_API_CASE_OGL3(x) case LM::RendererAPI::API::OpenGL3: x
#else
	#define RENDERER_API_CASE_OGL3(x) 
#endif

#ifdef OPENGL4
	#define RENDERER_API_CASE_OGL4(x) case LM::RendererAPI::API::OpenGL4: x
#else
	#define RENDERER_API_CASE_OGL4(x) 
#endif

namespace LM
{


	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
#ifdef OPENGL2
			OpenGL2,
#endif
#ifdef OPENGL3
			OpenGL3,
#endif
#ifdef OPENGL4
			OpenGL4,
#endif
#ifdef DIRECTX
			DirectX,
#endif
#ifdef VULKAN
			Vulkan,
#endif
		};
	public:
		static void Set(API _API) { s_API = _API; }
		static API Get() { return s_API; }
	protected:
		static inline API s_API = API::None;
	};

}