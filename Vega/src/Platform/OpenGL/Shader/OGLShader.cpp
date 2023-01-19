#include "OGLShader.h"


#include <string>
#include <fstream>

#include <GL/glew.h>

#include "Vega/Core/Assert.h"
#include "Vega/Debug/ConsoleLog.h"
#include "Vega/Renderers/RendererAPI.h"

namespace LM
{

	OGLShader::OGLShader(const ShaderLayout& _Layout)
	{
#ifdef DEBUG
		m_Layout = _Layout;
#endif

		m_ShaderID = Load(_Layout);
	}

	OGLShader::~OGLShader()
	{
		glDeleteProgram(m_ShaderID);
	}

	void OGLShader::Enable() const
	{
		glUseProgram(m_ShaderID);
	}

	void OGLShader::Disable() const
	{
		glUseProgram(0);
	}

	uint32_t OGLShader::Load(const ShaderLayout& _Layout)
	{
		uint32_t Program = glCreateProgram();
		std::vector<uint32_t> Shaders;
		for (uint32_t i = 0; i < _Layout.GetSources().size(); ++i)
		{
			Shaders.emplace_back(LoadShader(_Layout, i));
		}

		for (uint32_t ShaderID : Shaders)
		{
			glAttachShader(Program, ShaderID);
		}

		switch (RendererAPI::Get())
		{
			RENDERER_API_CASE_OGL2(
				for (uint32_t i = 0; i < _Layout.GetAttributes().size(); ++i)
				{
					glBindAttribLocation(Program, i, _Layout.GetAttributes()[i].Name.data());
				}
			);
		}

		glLinkProgram(Program);
		glValidateProgram(Program);

		int CompileRes;
		glGetProgramiv(Program, GL_LINK_STATUS, &CompileRes);
		if (CompileRes == GL_FALSE) {
			int Length;
			glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &Length);
			std::vector<char> Error(Length + 1024);
			glGetProgramInfoLog(Program, Length, &Length, &Error[0]);
			LOGE("Failed to link program!\n    ", &Error[0]);
			glDeleteProgram(Program);
			return 0;
		}

		for (uint32_t ShaderID : Shaders)
		{
			glDeleteShader(ShaderID);
		}

		return Program;
	}

	uint32_t OGLShader::GetType(ShaderSource::Type _Type)
	{
		switch (_Type)
		{
			case ShaderSource::Type::VERTEX:	return GL_VERTEX_SHADER;
			case ShaderSource::Type::FRAGMENT:	return GL_FRAGMENT_SHADER;
			case ShaderSource::Type::GEOMETRY:	return GL_GEOMETRY_SHADER;
		}
		CORE_ASSERT(false, "Unknown ShaderType in GetType");
		return 0;
	}

	std::string OGLShader::GetName(ShaderSource::Type _Type)
	{
		switch (_Type)
		{
			case ShaderSource::Type::VERTEX:	return "Vertex";
			case ShaderSource::Type::FRAGMENT:	return "Fragment";
			case ShaderSource::Type::GEOMETRY:	return "Geometry";
		}
		CORE_ASSERT(false, "Unknown ShaderType in GetName");
		return "None";
	}

	std::string OGLShader::LoadVersion()
	{
		switch (RendererAPI::Get())
		{
			RENDERER_API_CASE_OGL2(return "#version 120\n");
			RENDERER_API_CASE_OGL3(return "#version 330 core\n");
			RENDERER_API_CASE_OGL4(return "#version 450 core\n");
		}

		CORE_ASSERT(false, "Unknown ShaderDataType!");
		return "";
	}
	
	std::string OGLShader::GetAttributeTypeName(ShaderDataType _Type)
	{
		switch (_Type)
		{
			case LM::ShaderDataType::Float:		return "float";
			case LM::ShaderDataType::Float2:	return "vec2";
			case LM::ShaderDataType::Float3:	return "vec3";
			case LM::ShaderDataType::Float4:	return "vec4";
			case LM::ShaderDataType::Mat3:		return "mat3";
			case LM::ShaderDataType::Mat4:		return "mat4";
			case LM::ShaderDataType::Int:		return "int";
			case LM::ShaderDataType::Int2:		return "ivec2";
			case LM::ShaderDataType::Int3:		return "ivec3";
			case LM::ShaderDataType::Int4:		return "ivec4";
			case LM::ShaderDataType::Bool:		return "bool";
		}

		CORE_ASSERT(false, "Unknown ShaderDataType!");
		return "";
	}

	std::string OGLShader::LoadAttributes(const std::vector<ShaderAttribute>& _Attributes)
	{
		std::string Res;
		for (uint32_t i = 0; i < _Attributes.size(); ++i)
		{
			switch (RendererAPI::Get())
			{
#ifdef OPENGL2
			case RendererAPI::API::OpenGL2:
				Res += "attribute " + GetAttributeTypeName(_Attributes[i].Type) + " " + _Attributes[i].Name + ";\n";
				break;
#endif
#if defined(OPENGL3) || defined (OPENGL4)
#ifdef OPENGL3
			case RendererAPI::API::OpenGL3:
#endif
#ifdef OPENGL4
			case RendererAPI::API::OpenGL4:
#endif
				Res += "layout(location = " + std::to_string(i) + ") in " + GetAttributeTypeName(_Attributes[i].Type) + " " + _Attributes[i].Name + ";\n";
				break;
#endif
			}
		}

		return Res;
	}

	uint32_t OGLShader::LoadShader(const ShaderLayout& _Layout, uint32_t _SourceID)
	{
		const ShaderSource& Source = _Layout.GetSource(_SourceID);
		uint32_t Type = GetType(Source.GetType());
		std::string Name = GetName(Source.GetType());

		std::string SourceString = LoadVersion();
		if (Source.GetType() == ShaderSource::Type::VERTEX) SourceString += LoadAttributes(_Layout.GetAttributes());
		SourceString += (Source.GetLoadType() == ShaderSource::LoadType::FILEPATH ? LoadFile(Source.GetSource()) : Source.GetSource());
		const char* CharSource = SourceString.c_str();
		uint32_t Res = glCreateShader(Type);
		glShaderSource(Res, 1, &CharSource, NULL);
		glCompileShader(Res);
		
		LOGI(Name, " shader code: \n", SourceString);

		int CompileRes;
		glGetShaderiv(Res, GL_COMPILE_STATUS, &CompileRes);
		if (CompileRes == GL_FALSE) {
			GLint Length;
			glGetShaderiv(Res, GL_INFO_LOG_LENGTH, &Length);
			std::vector<char> Error(Length + 1024);
			glGetShaderInfoLog(Res, Length, &Length, &Error[0]);
			LOGE("Failed to compile ", Name, " shader!\n    ", &Error[0]);
			glDeleteShader(Res);
			return 0;
		}

		return Res;
	}

	std::string OGLShader::LoadFile(std::string_view _FilePath)
	{
		std::ifstream IfStream(_FilePath.data());
		if (!IfStream.is_open())
			LOGE("Can't Open Shader file: ", _FilePath);
		std::string Line;
		std::string Shader;

		while (getline(IfStream, Line))
		{
			//if (Line.find("#texture") < Line.size())
			//{
			//	int IDsCount;
			//	int MaxTextures;
			//	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &IDsCount);
			//	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MaxTextures);
			//
			//	const int PackCount = MaxTextures / IDsCount;
			//	const int NoneSimpleTextures = 1;
			//	const int TexturesToUse = MaxTextures - NoneSimpleTextures;
			//
			//	for (int i = 1; i <= PackCount; i++)
			//	{
			//		std::string TexName = "u_Textures" + std::to_string(i) + "[" + std::to_string(i != PackCount ? IDsCount : IDsCount - NoneSimpleTextures) + "]";
			//		Shader += "uniform sampler2D " + TexName + "; \n";
			//	}
			//
			//	Shader += "	vec4 GetTextureColor(int tid) \n";
			//	Shader += "	{ \n";
			//	Shader += "		if (tid == -1) \n";
			//	Shader += "			return vec4(1.0f, 1.0f, 1.0f, 1.0f); \n";
			//	Shader += "		switch (tid) \n";
			//	Shader += "		{ \n";
			//
			//	for (int i = 0; i < TexturesToUse; i++)
			//	{
			//		std::string caseStr = "case " + std::to_string(i) + ": \n";
			//		caseStr += "return texture(u_Textures" + std::to_string(i / IDsCount + 1) + "[" + std::to_string(i % IDsCount) + "], v_UV); \n";
			//
			//		Shader += caseStr; //i / IDsCount + 1; [i % IDsCount] ;
			//	}
			//	Shader +="		} \n";
			//	Shader +="	} \n";
			//
			//	continue;
			//}

			Shader += Line + "\n";
		}

		return Shader;
	}

	int OGLShader::GetUniformLocation(std::string_view _Name) const
	{
		if (m_UniformLocationCache.find(_Name.data()) != m_UniformLocationCache.end())
			return m_UniformLocationCache[_Name.data()];

		GLint location = glGetUniformLocation(m_ShaderID, _Name.data());
		m_UniformLocationCache[_Name.data()] = location;
		return location;
	}

	void OGLShader::SetUniform1f(std::string_view _Name, float value)
	{
		glUniform1f(GetUniformLocation(_Name), value);
	}
	void OGLShader::SetUniform1fv(std::string_view _Name, float* value, int count)
	{
		glUniform1fv(GetUniformLocation(_Name), count, value);
	}
	void OGLShader::SetUniform1i(std::string_view _Name, int value)
	{
		glUniform1i(GetUniformLocation(_Name), value);
	}
	void OGLShader::SetUniform1iv(std::string_view _Name, int* value, int count)
	{
		glUniform1iv(GetUniformLocation(_Name), count, value);
	}
	void OGLShader::SetUniform2f(std::string_view _Name, const glm::vec2& vector)
	{
		glUniform2f(GetUniformLocation(_Name), vector.x, vector.y);
	}
	void OGLShader::SetUniform3f(std::string_view _Name, const glm::vec3& vector)
	{
		glUniform3f(GetUniformLocation(_Name), vector.x, vector.y, vector.z);
	}
	void OGLShader::SetUniform4f(std::string_view _Name, const glm::vec4& vector)
	{
		glUniform4f(GetUniformLocation(_Name), vector.x, vector.y, vector.z, vector.w);
	}
	void OGLShader::SetUniformMat4(std::string_view _Name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(_Name), 1, GL_FALSE, &matrix[0][0]);
	}

}