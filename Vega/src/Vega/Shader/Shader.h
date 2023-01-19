#pragma once

#include <string_view>

#include "glm.hpp"

#include "Vega/Core/Base.h"
#include "ShaderLayout.h"

namespace LM
{

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Enable()	const = 0;
		virtual void Disable()	const = 0;

		virtual void SetUniform1f	(std::string_view _Name, float value) = 0;
		virtual void SetUniform1fv	(std::string_view _Name, float* value, int count) = 0;
		virtual void SetUniform1i	(std::string_view _Name, int value) = 0;
		virtual void SetUniform1iv	(std::string_view _Name, int* value, int count) = 0;
		virtual void SetUniform2f	(std::string_view _Name, const glm::vec2& vector) = 0;
		virtual void SetUniform3f	(std::string_view _Name, const glm::vec3& vector) = 0;
		virtual void SetUniform4f	(std::string_view _Name, const glm::vec4& vector) = 0;
		virtual void SetUniformMat4	(std::string_view _Name, const glm::mat4& matrix) = 0;

		//static Ref<Shader> Create(std::string_view _VertPath, std::string_view _FragPath);
		static Ref<Shader> Create(const ShaderLayout& _Layout);
	};

}