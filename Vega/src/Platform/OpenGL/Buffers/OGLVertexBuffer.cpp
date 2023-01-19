#include "OGLVertexBuffer.h"

#include <GL/glew.h>

namespace LM
{

	OGLVertexBuffer::OGLVertexBuffer(uint32_t _Size)
	{
		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ARRAY_BUFFER, _Size, nullptr, GL_DYNAMIC_DRAW);
	}

	OGLVertexBuffer::OGLVertexBuffer(const void* _Data, uint32_t _Size)
	{
		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ARRAY_BUFFER, _Size, _Data, GL_DYNAMIC_DRAW);
	}

	OGLVertexBuffer::~OGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void OGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	void OGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OGLVertexBuffer::SetData(const void* _Data, uint32_t _Size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, _Size, _Data);
	}

}