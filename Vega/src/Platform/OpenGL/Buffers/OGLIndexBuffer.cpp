#include "OGLIndexBuffer.h"

#include <GL/glew.h>

namespace LM
{

	OGLIndexBuffer::OGLIndexBuffer(uint32_t _Count)
		: m_Count(_Count)
	{
		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
	}

	OGLIndexBuffer::OGLIndexBuffer(const uint32_t* _Data, uint32_t _Count)
		: m_Count(_Count)
	{
		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), _Data, GL_DYNAMIC_DRAW);
	}

	OGLIndexBuffer::~OGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void OGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	}

	void OGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OGLIndexBuffer::SetIndices(const uint32_t* _Data, uint32_t _Count)
	{
		m_Count = _Count;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_Count * sizeof(uint32_t), _Data);
	}

}