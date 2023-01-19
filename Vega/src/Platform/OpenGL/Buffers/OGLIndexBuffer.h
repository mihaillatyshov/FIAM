#pragma once

#include "Vega/Buffers/IndexBuffer.h"

namespace LM
{

	class OGLIndexBuffer : public IndexBuffer
	{
	public:
		OGLIndexBuffer(uint32_t _Count);
		OGLIndexBuffer(const uint32_t* _Data, uint32_t _Count);
		virtual ~OGLIndexBuffer();

		virtual void Bind()		const override;
		virtual void Unbind()	const override;

		virtual void SetIndices(const uint32_t* _Data, uint32_t _Count) override;

		virtual uint32_t GetCount() const override { return m_Count; }
	protected:
		uint32_t m_BufferID;
		uint32_t m_Count;
	};

}