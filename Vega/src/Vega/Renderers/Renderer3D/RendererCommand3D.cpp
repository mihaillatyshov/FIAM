#include "RendererCommand3D.h"

#include "Renderer3D.h"

namespace LM
{

	void RendererCommand3D::UpdateRendererData(Ref<Renderer> _Renderer)
	{
		if (m_NeedClear)
		{
			Clear(_Renderer);
			m_NeedClear = false;
		}
		UpdateTexture2D(_Renderer);
		UpdateAddMesh(_Renderer);
	}

	void RendererCommand3D::Clear()
	{
		LOGI("RC CLEAR");
		m_NeedClear = true;
	}

	void RendererCommand3D::UpdateTexture2D(Ref<Renderer> _Renderer)
	{
		Ref<Renderer3D> R = StaticRefCast<Renderer3D>(_Renderer);

		Ref<Texture2D> Tex;
		while (m_AddTexture2D.TryPop(Tex))
		{
			R->LoadTexture(Tex);
		}
	}

	void RendererCommand3D::UpdateAddMesh(Ref<Renderer> _Renderer)
	{
		Ref<Renderer3D> R = StaticRefCast<Renderer3D>(_Renderer);
		
		std::pair<Ref<Mesh>, Ref<Transformation>> Object;
		while (m_AddMesh.TryPop(Object))
		{
			R->AddMesh(Object.first, Object.second);
		}
	}

	void RendererCommand3D::Clear(Ref<Renderer> _Renderer)
	{
		LOGI("RCR CLEAR");

		_Renderer->Clear();
	}

}