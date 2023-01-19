#include "OGLTexture2D.h"

#include <GL/glew.h>

namespace LM
{
	
	OGLTexture2D::OGLTexture2D(std::string_view _FileName, Texture::MASK _Mask)
	{

	}

	OGLTexture2D::~OGLTexture2D()
	{

	}

	void OGLTexture2D::Bind(uint32_t _SlotId)
	{

	}

	void OGLTexture2D::Unbind()
	{

	}

	void OGLTexture2D::OnAttach()
	{
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void OGLTexture2D::OnDetach()
	{
		glDeleteTextures(1, &m_TextureID);
	}

}