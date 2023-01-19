#include "OGL3VertexArray.h"

#include "gl/glew.h"

namespace LM
{

	OGL3VertexArray::OGL3VertexArray()
	{
		glGenVertexArrays(1, &m_BufferID);
	}

	OGL3VertexArray::~OGL3VertexArray()
	{
		glDeleteVertexArrays(1, &m_BufferID);
	}

	void OGL3VertexArray::Bind() const
	{
		glBindVertexArray(m_BufferID);
	}

	void OGL3VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OGL3VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer)
	{
		Bind();
		ProcessVertexBuffer(_VertexBuffer);
		m_VertexBuffers.push_back(_VertexBuffer);
	}

	void OGL3VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer)
	{
		Bind();
		_IndexBuffer->Bind();

		m_IndexBuffer = _IndexBuffer;
	}

}