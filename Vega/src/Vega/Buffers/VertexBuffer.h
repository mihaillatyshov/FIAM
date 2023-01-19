#pragma once

#include "BufferLayout.h"

namespace LM
{

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind()		const = 0;
		virtual void Unbind()	const = 0;

		virtual void SetData(const void* _Data, uint32_t _Size) = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& _Layout) = 0;

		static Ref<VertexBuffer> Create(uint32_t _Size);
		static Ref<VertexBuffer> Create(const void* _Data, uint32_t _Size);
	};

}