#pragma once

#include <unordered_map>

#include "Vega/Shader/Shader.h"

namespace LM
{

	class OGLShader : public Shader
	{
	public:
		//OGLShader(std::string_view _VertPath, std::string_view _FragPath);
		OGLShader(const ShaderLayout& _Layout);
		virtual ~OGLShader();

		virtual void Enable() const override;
		virtual void Disable() const override;

		virtual void SetUniform1f	(std::string_view _Name, float value) override;
		virtual void SetUniform1fv	(std::string_view _Name, float* value, int count) override;
		virtual void SetUniform1i	(std::string_view _Name, int value) override;
		virtual void SetUniform1iv	(std::string_view _Name, int* value, int count) override;
		virtual void SetUniform2f	(std::string_view _Name, const glm::vec2& vector) override;
		virtual void SetUniform3f	(std::string_view _Name, const glm::vec3& vector) override;
		virtual void SetUniform4f	(std::string_view _Name, const glm::vec4& vector) override;
		virtual void SetUniformMat4	(std::string_view _Name, const glm::mat4& matrix) override;
	protected:
		uint32_t Load(const ShaderLayout& _Layout);
		uint32_t GetType(ShaderSource::Type _Type);
		std::string GetName(ShaderSource::Type _Type);
		std::string LoadVersion();
		std::string GetAttributeTypeName(ShaderDataType _Type);
		std::string LoadAttributes(const std::vector<ShaderAttribute>& _Attributes);
		uint32_t LoadShader(const ShaderLayout& _Layout, uint32_t _SourceID);
		std::string LoadFile(std::string_view _FilePath);
		int GetUniformLocation(std::string_view name) const;
	protected:
		uint32_t m_ShaderID;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;
#ifdef DEBUG
		ShaderLayout m_Layout;
#endif
	};

}