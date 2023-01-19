#pragma once

#include "glm.hpp"

#include "Material.h"
#include "Vega/Textures/Texture2D.h"

namespace LM
{

	struct Material3DProps
	{
		Ref<Texture2D> DiffuseTexture;
		Ref<Texture2D> SpecularMap;
		Ref<Texture2D> NormalMap;
		glm::vec4 DiffuseColor;
		glm::vec4 SpecularCoef;
	};

	class Material3D : public Material
	{
	public:
		Material3D(Material3DProps _Props) : m_Props(_Props) { }
	protected:
		Material3DProps m_Props;
	};

}