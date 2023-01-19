#include "OGL4ShaderStorageBuffer.h"

#include <GL/glew.h>

namespace LM
{

	OGL4ShaderStorageBuffer::OGL4ShaderStorageBuffer(uint32_t _Size)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, _Size, nullptr, GL_DYNAMIC_DRAW);
	}

	OGL4ShaderStorageBuffer::OGL4ShaderStorageBuffer(const void* _Data, uint32_t _Size)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, _Size, _Data, GL_DYNAMIC_DRAW);
	}

	OGL4ShaderStorageBuffer::~OGL4ShaderStorageBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void OGL4ShaderStorageBuffer::Bind(uint32_t _SlotId) const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _SlotId, m_BufferID);
	}

	void OGL4ShaderStorageBuffer::Unbind() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void OGL4ShaderStorageBuffer::SetData(const void* _Data, uint32_t _Size)
	{
		glNamedBufferSubData(m_BufferID, 0, _Size, _Data);
	}

}