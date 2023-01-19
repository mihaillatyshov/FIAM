#pragma once

#include "Vega/Buffers/VertexBuffer.h"

namespace LM
{

	class OGLVertexBuffer : public VertexBuffer
	{
	public:
		OGLVertexBuffer(uint32_t _Size);
		OGLVertexBuffer(const void* _Data, uint32_t _Size);
		virtual ~OGLVertexBuffer();

		virtual void Bind()		const override;
		virtual void Unbind()	const override;

		virtual void SetData(const void* _Data, uint32_t _Size) override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& _Layout) override { m_Layout = _Layout; }
	protected:
		uint32_t m_BufferID;
		BufferLayout m_Layout;
	};

}