#include "OGL4IndexBuffer.h"

#include <GL/glew.h>

namespace LM
{

	OGL4IndexBuffer::OGL4IndexBuffer(uint32_t _Count)
		: m_Count(_Count)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, m_Count * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
	}

	OGL4IndexBuffer::OGL4IndexBuffer(const uint32_t* _Data, uint32_t _Count)
		: m_Count(_Count)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, m_Count * sizeof(uint32_t), _Data, GL_DYNAMIC_DRAW);
	}

	OGL4IndexBuffer::~OGL4IndexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void OGL4IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	}

	void OGL4IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OGL4IndexBuffer::SetIndices(const uint32_t* _Data, uint32_t _Count)
	{
		m_Count = _Count;
		glNamedBufferSubData(m_BufferID, 0, m_Count * sizeof(uint32_t), _Data);
	}

}