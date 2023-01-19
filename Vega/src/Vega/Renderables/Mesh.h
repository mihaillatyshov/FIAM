#pragma once

#include <vector>

#include "Renderable3D.h"
#include "Vega/Materials/Material3D.h"

namespace LM
{

	struct MeshVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec3 Normal;
		glm::vec3 Tangent;
	};

	class Mesh : public Renderable3D
	{
	public:
		Mesh(const std::vector<MeshVertex>& _Vertices, const std::vector<uint32_t>& _Indices) : m_Vertices(_Vertices), m_Indices(_Indices) { }

		const std::vector<MeshVertex>& GetVertices() const { return m_Vertices; }
		const std::vector<uint32_t>& GetIndices() const { return m_Indices; }
		const Ref<Material>& GetMaterial() const { return m_Material; }

		virtual void Add(Ref<Renderable> _Renderable, Ref<Transformation> _Transformation, Ref<RendererCommand> _RendererCommand) override;
	protected:
		std::vector<MeshVertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		
		Ref<Material3D> m_Material;
	};

}