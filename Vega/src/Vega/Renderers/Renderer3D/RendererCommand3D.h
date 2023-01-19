#pragma once

#include <queue>
#include <mutex>

#include "Vega/Renderers/RendererCommand.h"
#include "Vega/Renderables/Mesh.h"
#include "Vega/Threads/QueueMT.h"

namespace LM
{

	class RendererCommand3D : public RendererCommand
	{
	public:
		virtual void Add(Ref<Mesh> _Mesh, Ref<Transformation> _Transformation) { m_AddMesh.Push(std::make_pair(_Mesh, _Transformation)); }
		virtual void UpdateRendererData(Ref<Renderer> _Renderer) override;
		virtual void Clear() override;
	protected:
		virtual void UpdateTexture2D(Ref<Renderer> _Renderer);
		virtual void UpdateAddMesh(Ref<Renderer> _Renderer);
		virtual void Clear(Ref<Renderer> _Renderer);
	protected:
		QueueMT<std::pair<Ref<Mesh>, Ref<Transformation>>> m_AddMesh;
		bool m_NeedClear = false;
	};

}