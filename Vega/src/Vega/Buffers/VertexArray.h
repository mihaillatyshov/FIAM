#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace LM
{

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind()		const = 0;
		virtual void Unbind()	const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer) = 0;
		virtual void SetIndexBuffer (const Ref<IndexBuffer>&  _IndexBuffer)  = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};

}