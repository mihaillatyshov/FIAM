#pragma once

#include "Vega/Buffers/VertexArray.h"

namespace LM
{

	class OGLVertexArray : public VertexArray
	{
	public:
		OGLVertexArray() = default;
		virtual ~OGLVertexArray() = default;

		void ProcessVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer) const;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	protected:
		mutable uint32_t m_VertexBufferIndex = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}