#pragma once
#include "glm.hpp"

#include "Vega/Renderers/Renderer3D/Renderer3D.h"
#include "Vega/Renderables/Mesh.h"
#include "Vega/Shader/Shader.h"
#include "Vega/Buffers/VertexArray.h"
#include "Vega/Buffers/ShaderStorageBuffer.h"
#include "Vega/Buffers/FrameBuffer.h"

namespace LM
{

	class OGL4Renderer3D : public Renderer3D
	{
	public:
		OGL4Renderer3D();
		virtual ~OGL4Renderer3D() { }

		virtual void AddMesh(Ref<Mesh> _Mesh, Ref<Transformation> _Transformation) override;
		virtual void LoadTexture(Ref<Texture2D> _Texture) override;

		virtual void* GetTextureResult() const override { return (void*)m_ScreenFrameBuffer->GetTextureId(); };

		virtual void Clear() override;

		virtual void OnEvent(const Ref<Event>& _Event) override;
		virtual void Draw() override;
	protected:
		struct DrawElementsIndirectCommand 
		{
			uint32_t	Count;
			uint32_t	InstanceCount;
			uint32_t	FirstIndex;
			int			BaseVertex;
			uint32_t	BaseInstance;
		};
	protected:
		Ref<Shader> m_Shader;
		std::vector<uint64_t> m_Materials;
		Ref<ShaderStorageBuffer> m_MaterialsSSBO;
		Ref<VertexBuffer> m_VerticesVBO;
		Ref<VertexBuffer> m_MaterailsVBO;
		Ref<VertexBuffer> m_TransformationsVBO;
		Ref<IndexBuffer> m_IBO;
		Ref<VertexArray> m_VertexArray;
		bool m_MeshesNeedUpdate = false;
		std::vector<Ref<Mesh>> m_Meshes;
		std::vector<MeshVertex> m_VerticesBuffer;
		std::vector<int> m_MaterialsBuffer;
		std::vector<int> m_TransformationsBuffer;
		std::vector<uint32_t> m_IndicesBuffer;
		std::vector<DrawElementsIndirectCommand> m_Commands;

		std::vector<Ref<Texture2D>> m_Textures;

		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Shader> m_ScreenShader;
		Ref<VertexArray> m_ScreenVertexArray;
		Ref<FrameBuffer> m_ScreenFrameBuffer;

	};

}