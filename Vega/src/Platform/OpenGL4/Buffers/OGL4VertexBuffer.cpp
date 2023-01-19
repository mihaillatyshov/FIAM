#include "OGL4VertexBuffer.h"

#include <GL/glew.h>

namespace LM
{

	OGL4VertexBuffer::OGL4VertexBuffer(uint32_t _Size)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, _Size, nullptr, GL_DYNAMIC_DRAW);
	}

	OGL4VertexBuffer::OGL4VertexBuffer(const void* _Data, uint32_t _Size)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, _Size, _Data, GL_DYNAMIC_DRAW);
	}

	OGL4VertexBuffer::~OGL4VertexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void OGL4VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	void OGL4VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OGL4VertexBuffer::SetData(const void* _Data, uint32_t _Size)
	{
		glNamedBufferSubData(m_BufferID, 0, _Size, _Data);
	}

}