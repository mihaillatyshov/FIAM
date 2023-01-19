#pragma once

#include "Vega/Buffers/ShaderStorageBuffer.h"

namespace LM
{

	class OGL4ShaderStorageBuffer : public ShaderStorageBuffer
	{
	public:
		OGL4ShaderStorageBuffer(uint32_t _Size);
		OGL4ShaderStorageBuffer(const void* _Data, uint32_t _Size);
		virtual ~OGL4ShaderStorageBuffer();

		virtual void Bind(uint32_t _SlotId)	const override;
		virtual void Unbind()				const override;

		virtual void SetData(const void* _Data, uint32_t _Size) override;

		inline uint32_t GetBufferID() const { return m_BufferID; }
	protected:
		uint32_t m_BufferID;
	};

}