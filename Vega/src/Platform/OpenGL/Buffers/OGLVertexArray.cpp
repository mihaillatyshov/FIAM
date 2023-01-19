#include "OGLVertexArray.h"

#include <GL/glew.h>

#include "Vega/Renderers/RendererAPI.h"

namespace LM
{

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType _Type)
	{
		switch (_Type)
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
		}

		CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	void OGLVertexArray::ProcessVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer) const
	{
		CORE_ASSERT(_VertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		_VertexBuffer->Bind();

		const auto& Layout = _VertexBuffer->GetLayout();
		for (const auto& Element : Layout)
		{
			switch (Element.Type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(	m_VertexBufferIndex, 
											Element.GetComponentCount(),
											ShaderDataTypeToOpenGLBaseType(Element.Type),
											Element.Normalized ? GL_TRUE : GL_FALSE,
											Layout.GetStride(),
											(const void*)Element.Offset);
					if (Layout.HasDivisor())
					{
						switch (RendererAPI::Get())
						{
							RENDERER_API_CASE_OGL2(CORE_ASSERT(false, "Test in OpenGL 2 with glVertexAttribDivisor!!!")); break;
						}
						glVertexAttribDivisor(m_VertexBufferIndex, Layout.GetDivisor());
						LOGW("[OGL3]: SET DIV F");
					}
					m_VertexBufferIndex++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribIPointer(	m_VertexBufferIndex,
											Element.GetComponentCount(),
											ShaderDataTypeToOpenGLBaseType(Element.Type),
											Layout.GetStride(),
											(const void*)Element.Offset);
					if (Layout.HasDivisor())
					{
						switch (RendererAPI::Get())
						{
							RENDERER_API_CASE_OGL2(CORE_ASSERT(false, "Test in OpenGL 2 with glVertexAttribDivisor!!!")); break;
						}
						glVertexAttribDivisor(m_VertexBufferIndex, Layout.GetDivisor());
						LOGW("[OGL3]: SET DIV I");
					}
					m_VertexBufferIndex++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					uint8_t Count = Element.GetComponentCount();
					for (uint8_t i = 0; i < Count; ++i)
					{
						glEnableVertexAttribArray(m_VertexBufferIndex);
						glVertexAttribPointer(	m_VertexBufferIndex,
												Count,
												ShaderDataTypeToOpenGLBaseType(Element.Type),
												Element.Normalized ? GL_TRUE : GL_FALSE,
												Layout.GetStride(),
												(const void*)(Element.Offset + sizeof(float) * Count * i));
						if (Layout.HasDivisor())
						{
							switch (RendererAPI::Get())
							{
								RENDERER_API_CASE_OGL2(CORE_ASSERT(false, "Test in OpenGL 2 with glVertexAttribDivisor!!!")); break;
							}
							glVertexAttribDivisor(m_VertexBufferIndex, Layout.GetDivisor());
							LOGW("[OGL3]: SET DIV M");
						}
						m_VertexBufferIndex++;
					}
					break;
				}
			}
		}
	}


}