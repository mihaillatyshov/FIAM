#pragma once

#include "Platform/OpenGL/Buffers/OGLVertexArray.h"

namespace LM
{

	class OGL3VertexArray : public OGLVertexArray
	{
	public:
		OGL3VertexArray();
		virtual ~OGL3VertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer) override;
	protected:
		uint32_t m_BufferID;
	};

}