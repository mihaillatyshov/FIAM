#include "OGLCalcTextureParameters.h"

#include <GL/glew.h>

namespace LM
{

	int OGLCalcTextureParameters::InputType(Texture::MASK _Mask)
	{
		if (_Mask & Texture::MASK::NO_SRGB) return GL_RGBA;

		return GL_SRGB_ALPHA;
	}
	
	int OGLCalcTextureParameters::OutputType(Texture::MASK _Mask)
	{
		if (_Mask & Texture::MASK::NO_ALPHA) return GL_RGB;

		return GL_RGBA;
	}
	
	int OGLCalcTextureParameters::MagFilter(Texture::MASK _Mask)
	{
		if (_Mask & Texture::MASK::MAG_NEAREST) return GL_NEAREST;

		return GL_LINEAR;
	}
	
	int OGLCalcTextureParameters::MinFilter(Texture::MASK _Mask)
	{
		if (_Mask & Texture::MASK::MIN_NEAREST)					return GL_NEAREST;
		if (_Mask & Texture::MASK::MIN_LINEAR_MIPMAP_LINEAR)	return GL_LINEAR_MIPMAP_LINEAR;
		if (_Mask & Texture::MASK::MIN_LINEAR_MIPMAP_NEAREST)	return GL_LINEAR_MIPMAP_NEAREST;
		if (_Mask & Texture::MASK::MIN_NEAREST_MIPMAP_LINEAR)	return GL_NEAREST_MIPMAP_LINEAR;
		if (_Mask & Texture::MASK::MIN_NEAREST_MIPMAP_NEAREST)	return GL_NEAREST_MIPMAP_NEAREST;

		return GL_LINEAR;
	}
	
	int OGLCalcTextureParameters::WrapS(Texture::MASK _Mask)
	{
		if (_Mask & Texture::MASK::S_MIRRORED_REPEAT)	return GL_MIRRORED_REPEAT;
		if (_Mask & Texture::MASK::S_CLAMP_TO_EDGE)		return GL_LINEAR_MIPMAP_LINEAR;
		if (_Mask & Texture::MASK::S_CLAMP_TO_BORDER)	return GL_LINEAR_MIPMAP_NEAREST;

		return GL_REPEAT;
	}
	
	int OGLCalcTextureParameters::WrapT(Texture::MASK _Mask)
	{
		if (_Mask & Texture::MASK::T_MIRRORED_REPEAT)	return GL_MIRRORED_REPEAT;
		if (_Mask & Texture::MASK::T_CLAMP_TO_EDGE)		return GL_LINEAR_MIPMAP_LINEAR;
		if (_Mask & Texture::MASK::T_CLAMP_TO_BORDER)	return GL_LINEAR_MIPMAP_NEAREST;

		return GL_REPEAT;
	}
	
	int OGLCalcTextureParameters::WrapR(Texture::MASK _Mask)
	{
		if (_Mask & Texture::MASK::R_MIRRORED_REPEAT)	return GL_MIRRORED_REPEAT;
		if (_Mask & Texture::MASK::R_CLAMP_TO_EDGE)		return GL_LINEAR_MIPMAP_LINEAR;
		if (_Mask & Texture::MASK::R_CLAMP_TO_BORDER)	return GL_LINEAR_MIPMAP_NEAREST;

		return GL_REPEAT;
	}

	bool OGLCalcTextureParameters::HasMipmap(Texture::MASK _Mask)
	{
		return	_Mask & Texture::MASK::MIN_LINEAR_MIPMAP_LINEAR		||
				_Mask & Texture::MASK::MIN_LINEAR_MIPMAP_NEAREST 	||
				_Mask & Texture::MASK::MIN_NEAREST_MIPMAP_LINEAR 	||
				_Mask & Texture::MASK::MIN_NEAREST_MIPMAP_NEAREST;

	}

}