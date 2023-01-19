#pragma once

#include <cstdint>

#include "Vega/Core/Base.h"

namespace LM
{

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind()		const = 0;
		virtual void Unbind()	const = 0;

		virtual void SetIndices(const uint32_t* _Data, uint32_t Count) = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t _Count);
		static Ref<IndexBuffer> Create(const uint32_t* _Data, uint32_t _Count);
	};

}