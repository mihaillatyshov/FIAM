#pragma once

#include "Vega/Core/Base.h"
#include "Vega/Renderables/Renderable.h"

namespace LM
{

	class GameObject
	{
	public:

	protected:
		std::vector<Ref<GameObject>> m_GameObjects;
		std::vector<Ref<Renderable>> m_Renderables;
	};

}