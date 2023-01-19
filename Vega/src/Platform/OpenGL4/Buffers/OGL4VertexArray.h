#pragma once

#include "Vega/Buffers/VertexArray.h"

namespace LM
{

	class OGL4VertexArray : public VertexArray
	{
	public:
		OGL4VertexArray();
		virtual ~OGL4VertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	protected:
		uint32_t m_BufferID;
		mutable uint32_t m_VertexBufferIndex = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}