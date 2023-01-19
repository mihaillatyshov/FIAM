#pragma once

#include <vector>

#include "Vega/Core/Base.h"
#include "GameObject.h"

namespace LM
{

	class Scene
	{
	public:

	protected:
		std::vector<Ref<GameObject>> m_Objects;
	};

}