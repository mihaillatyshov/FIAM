#include "ShaderLayout.h"

#include "Vega/Core/Assert.h"

namespace LM
{

	ShaderLayout::ShaderLayout(	const std::initializer_list<ShaderAttribute>& _Attributes,
								const std::initializer_list<ShaderSource>& _Sources)
		: m_Attributes(_Attributes), m_Sources(_Sources)
	{
		CalculateAttributesLocation();
	}

	void ShaderLayout::CalculateAttributesLocation()
	{
		CORE_ASSERT(m_Attributes.size() != 0, "No Attributes in Shader Layout!")

		uint32_t Location = 0;
		for (auto& Attribute : m_Attributes)
		{
			Attribute.Location = Location;
			Location += GetAttributeLocationOffset(Attribute.Type);
		}
	}

	uint32_t ShaderLayout::GetAttributeLocationOffset(ShaderDataType _Type)
	{
		switch (_Type)
		{
			case ShaderDataType::Mat3: return 3;
			case ShaderDataType::Mat4: return 4;
			default: return 1;
		}
	}

}