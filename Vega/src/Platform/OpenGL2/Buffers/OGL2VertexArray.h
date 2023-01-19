#pragma once

#include "Platform/OpenGL/Buffers/OGLVertexArray.h"

namespace LM
{
	class OGL2VertexArray : public OGLVertexArray
	{
	public:
		OGL2VertexArray();
		virtual ~OGL2VertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer) override;
	};

}