#pragma once

#include "Layer.h"
#include "Vega/Textures/Texture2D.h"
#include <Vega/FIAM/FIAM.h>

namespace LM
{

	class Layer3D : public Layer
	{
	public:

		virtual void OnAttach() override;
		virtual void OnEvent(Ref<Event> _Event) override;
	protected:
		Ref<Texture2D> Tex;
		Ref<FIAM> m_FIAM;

		bool m_MoveUp = false;
	};

}