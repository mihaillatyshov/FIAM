#pragma once

#include <string>
#include <vector>

#include "Vega/Core/Base.h"
#include "Vega/Core/Assert.h"
#include "Vega/Shader/ShaderDataType.h"

namespace LM 
{

	struct BufferElement
	{
		BufferElement() = default;

		BufferElement(ShaderDataType _Type, std::string_view _Name, bool _Normalized = false)
			: Name(_Name), Type(_Type), Size(ShaderDataTypeSize(_Type)), Offset(0), Normalized(_Normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3;
				case ShaderDataType::Mat4:    return 4;
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool:    return 1;
			}

			CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}

		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;
	};


	class BufferLayout
	{
	public:
		BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferElement>& _Elements, uint32_t _Divisor = 0)
			: m_Elements(_Elements), m_Divisor(_Divisor)
		{
			CalculateOffsetsAndStride();
		}

		bool HasDivisor() const { return m_Divisor; }
		uint32_t GetDivisor() const { return m_Divisor; }

		inline uint32_t GetStride() const { return m_Stride; }

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin()	{ return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end()		{ return m_Elements.end(); }

		std::vector<BufferElement>::const_iterator begin()	const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end()	const { return m_Elements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t Offset = 0;
			m_Stride = 0;
			for (auto& Element : m_Elements)
			{
				Element.Offset = Offset;
				Offset += Element.Size;
				m_Stride += Element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Divisor = 0;
		uint32_t m_Stride = 0;
	};
}
