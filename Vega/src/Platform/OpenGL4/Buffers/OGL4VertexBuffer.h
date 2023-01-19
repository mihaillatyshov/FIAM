#pragma once

#include "Vega/Buffers/VertexBuffer.h"

namespace LM
{

	class OGL4VertexBuffer : public VertexBuffer
	{
	public:
		OGL4VertexBuffer(uint32_t _Size);
		OGL4VertexBuffer(const void* _Data, uint32_t _Size);
		virtual ~OGL4VertexBuffer();

		virtual void Bind()		const override;
		virtual void Unbind()	const override;

		virtual void SetData(const void* _Data, uint32_t _Size) override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& _Layout) override { m_Layout = _Layout; }

		inline uint32_t GetBufferID() const { return m_BufferID; }
	protected:
		uint32_t m_BufferID;
		BufferLayout m_Layout;
	};

}