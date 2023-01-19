#include "OGL2VertexArray.h"

namespace LM
{

	OGL2VertexArray::OGL2VertexArray()
	{

	}

	OGL2VertexArray::~OGL2VertexArray()
	{

	}

	void OGL2VertexArray::Bind() const
	{
		m_VertexBufferIndex = 0;

		for (const auto& Buffer : m_VertexBuffers)
		{
			ProcessVertexBuffer(Buffer);
		}
		m_IndexBuffer->Bind();
	}

	void OGL2VertexArray::Unbind() const
	{

	}

	void OGL2VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer)
	{
		m_VertexBuffers.push_back(_VertexBuffer);
	}

	void OGL2VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer)
	{
		m_IndexBuffer = _IndexBuffer;
	}

}