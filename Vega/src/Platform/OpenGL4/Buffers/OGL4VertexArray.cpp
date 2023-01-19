#include "OGL4VertexArray.h"

#include <GL/glew.h>

#include "OGL4VertexBuffer.h"
#include "OGL4IndexBuffer.h"

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

		//return GL_VERTEX_ATTRIB_ARRAY_DIVISOR;
		CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OGL4VertexArray::OGL4VertexArray()
	{
		glCreateVertexArrays(1, &m_BufferID);
	}

	OGL4VertexArray::~OGL4VertexArray()
	{
		glDeleteVertexArrays(1, &m_BufferID);
	}

	void OGL4VertexArray::Bind() const
	{
		glBindVertexArray(m_BufferID);
	}

	void OGL4VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OGL4VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer)
	{
		CORE_ASSERT(_VertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		const auto& Layout = _VertexBuffer->GetLayout();

		int BuffersOffset = 0;
		//for (const auto& VB : m_VertexBuffers)
		//{
		//	BuffersOffset += VB->GetLayout().GetStride();
		//}

		glVertexArrayVertexBuffer(m_BufferID, m_VertexBuffers.size(), StaticRefCast<OGL4VertexBuffer>(_VertexBuffer)->GetBufferID(), BuffersOffset, Layout.GetStride());
		for (const auto& Element : Layout)
		{
			switch (Element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexArrayAttrib(m_BufferID, m_VertexBufferIndex);
				glVertexArrayAttribFormat(m_BufferID, m_VertexBufferIndex,
					Element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(Element.Type),
					Element.Normalized ? GL_TRUE : GL_FALSE,
					Element.Offset);
				glVertexArrayAttribBinding(m_BufferID, m_VertexBufferIndex, m_VertexBuffers.size());
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexArrayAttrib(m_BufferID, m_VertexBufferIndex);
				glVertexArrayAttribIFormat(m_BufferID, m_VertexBufferIndex,
					Element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(Element.Type),
					Element.Offset);
				glVertexArrayAttribBinding(m_BufferID, m_VertexBufferIndex, m_VertexBuffers.size());
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t Count = Element.GetComponentCount();
				for (uint8_t i = 0; i < Count; ++i)
				{
					glEnableVertexArrayAttrib(m_BufferID, m_VertexBufferIndex);
					glVertexArrayAttribFormat(m_BufferID, m_VertexBufferIndex,
						Count,
						ShaderDataTypeToOpenGLBaseType(Element.Type),
						Element.Normalized ? GL_TRUE : GL_FALSE,
						Element.Offset + sizeof(float) * Count * i);
					glVertexArrayAttribBinding(m_BufferID, m_VertexBufferIndex, m_VertexBuffers.size());
					m_VertexBufferIndex++;
				}
				break;
			}
			}
		}
		if (Layout.HasDivisor())
		{
			glVertexArrayBindingDivisor(m_BufferID, m_VertexBuffers.size(), Layout.GetDivisor());
			LOGW("[OGL4]: SET DIV F");
		}
		m_VertexBuffers.push_back(_VertexBuffer);
	}

	void OGL4VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer)
	{
		glVertexArrayElementBuffer(m_BufferID, StaticRefCast<OGL4IndexBuffer>(_IndexBuffer)->GetBufferID());
		m_IndexBuffer = _IndexBuffer;
	}

}