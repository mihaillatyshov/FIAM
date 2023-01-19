#include "Mesh.h"

#include "Vega/Renderers/Renderer3D/RendererCommand3D.h"

namespace LM
{

	void Mesh::Add(Ref<Renderable> _Renderable, Ref<Transformation> _Transformation, Ref<RendererCommand> _RendererCommand)
	{
		Ref<RendererCommand3D> RC = StaticRefCast<RendererCommand3D>(_RendererCommand);

		RC->Add(StaticRefCast<Mesh>(_Renderable), _Transformation);
	}

}
