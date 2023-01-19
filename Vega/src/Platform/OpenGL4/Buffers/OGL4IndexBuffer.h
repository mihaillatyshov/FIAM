#pragma once

#include "Vega/Buffers/IndexBuffer.h"

namespace LM
{

	class OGL4IndexBuffer : public IndexBuffer
	{
	public:
		OGL4IndexBuffer(uint32_t _Count);
		OGL4IndexBuffer(const uint32_t* _Data, uint32_t _Count);
		virtual ~OGL4IndexBuffer();

		virtual void Bind()		const override;
		virtual void Unbind()	const override;

		virtual void SetIndices(const uint32_t* _Data, uint32_t _Count) override;

		inline uint32_t GetBufferID() const { return m_BufferID; }
		virtual uint32_t GetCount() const override { return m_Count; }
	protected:
		uint32_t m_BufferID;
		uint32_t m_Count;
	};

}