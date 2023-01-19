#pragma once

#include "Vega/Core/Base.h"

namespace LM
{

	class ShaderStorageBuffer
	{
	public:
		virtual ~ShaderStorageBuffer() = default;

		virtual void Bind(uint32_t _SlotId)	const = 0;
		virtual void Unbind()				const = 0;

		virtual void SetData(const void* _Data, uint32_t _Size) = 0;

		static Ref<ShaderStorageBuffer> Create(uint32_t _Size);
		static Ref<ShaderStorageBuffer> Create(const void* _Data, uint32_t _Size);
	};

}