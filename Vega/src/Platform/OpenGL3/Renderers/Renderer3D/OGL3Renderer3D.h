#pragma once
#include "glm.hpp"

#include "Vega/Renderers/Renderer3D/Renderer3D.h"
#include "Vega/Renderables/Mesh.h"
#include "Vega/Shader/Shader.h"
#include "Vega/Buffers/VertexArray.h"

namespace LM
{

	class OGL3Renderer3D : public Renderer3D
	{
	public:
		OGL3Renderer3D();

		virtual void AddMesh(Ref<Mesh> _Mesh, Ref<Transformation> _Transformation) override;
		virtual void LoadTexture(Ref<Texture2D> _Texture) override { }
		
		virtual void* GetTextureResult() const override { return (void*)0; };

		virtual void Clear() override { }
		virtual void Draw() override;
	protected:
		Ref<Shader> m_Shader;
		Ref<VertexBuffer> m_VerticesVBO;
		Ref<VertexBuffer> m_MaterailsVBO;
		Ref<VertexBuffer> m_TransformationsVBO;
		Ref<IndexBuffer> m_IBO;
		Ref<VertexArray> m_VertexArray;
		std::vector<MeshVertex> m_VerticesBuffer;
		std::vector<int> m_MaterialsBuffer;
		std::vector<int> m_TransformationsBuffer;
		std::vector<uint32_t> m_IndicesBuffer;
	};

}