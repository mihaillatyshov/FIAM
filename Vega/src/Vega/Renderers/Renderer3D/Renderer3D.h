#pragma once

#include "Vega/Debug/ConsoleLog.h"
#include "Vega/Renderers/Renderer.h"
#include "RendererCommand3D.h"
#include "Vega/Renderables/Mesh.h"

namespace LM
{

	class Renderer3D : public Renderer
	{
	public:
		virtual ~Renderer3D() = default;

		static Ref<Renderer3D> Create();

		virtual Ref<RendererCommand> CreateRendererCommand() override { return CreateRef<RendererCommand3D>(); }

		virtual void AddMesh(Ref<Mesh> _Mesh, Ref<Transformation> _Transformation) = 0;
		virtual void LoadTexture(Ref<Texture2D> _Texture) = 0;
	};

}