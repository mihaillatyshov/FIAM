#pragma once

#include <vector>

#include "ShaderDataType.h"

namespace LM
{

	struct ShaderAttribute
	{
		ShaderAttribute() = default;

		ShaderAttribute(ShaderDataType _Type, std::string_view _Name)
			: Name(_Name), Type(_Type), Location(0)
		{
		}

		std::string Name;
		ShaderDataType Type;
		uint32_t Location;
	};

	class ShaderSource
	{
	public:
		enum class Type
		{
			VERTEX,
			FRAGMENT,
			GEOMETRY
		};

		enum class LoadType
		{
			FILEPATH,
			SOURCE
		};

		ShaderSource(Type _Type, std::pair<std::string, LoadType> _Source)
			: m_Type(_Type), m_LoadType(_Source.second), m_Source(_Source.first) { }

		inline Type GetType()					const { return m_Type; }
		inline LoadType GetLoadType()			const { return m_LoadType; }
		inline const std::string& GetSource()	const { return m_Source; }
	public:
		static std::pair<std::string, LoadType> FromSource(std::string_view _Source) { return { _Source.data(), LoadType::SOURCE }; }
		static std::pair<std::string, LoadType> FromFile(std::string_view _File)	 { return { _File.data(),	LoadType::FILEPATH}; }
	protected:
		Type m_Type;
		LoadType m_LoadType;
		std::string m_Source;
	};

	class ShaderLayout
	{
	public:
		ShaderLayout() = default;
		ShaderLayout(	const std::initializer_list<ShaderAttribute>& _Attributes, 
						const std::initializer_list<ShaderSource>& _Sources);

		inline const std::vector<ShaderAttribute>& GetAttributes()	const { return m_Attributes; }
		inline const std::vector<ShaderSource>& GetSources()		const { return m_Sources; }
		inline const ShaderSource& GetSource(uint32_t _SourceID)	const { return m_Sources[_SourceID]; }
	protected:
		void CalculateAttributesLocation();
		uint32_t GetAttributeLocationOffset(ShaderDataType _Type);
	protected:
		std::vector<ShaderAttribute> m_Attributes;
		std::vector<ShaderSource> m_Sources;
	};

}