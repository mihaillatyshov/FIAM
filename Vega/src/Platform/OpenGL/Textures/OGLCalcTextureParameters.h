#pragma once

#include "Vega/Textures/Texture.h"

namespace LM
{

	class OGLCalcTextureParameters
	{
	public:
		static int InputType(Texture::MASK _Mask);
		static int OutputType(Texture::MASK _Mask);
		static int MagFilter(Texture::MASK _Mask);
		static int MinFilter(Texture::MASK _Mask);
		static int WrapS(Texture::MASK _Mask);
		static int WrapT(Texture::MASK _Mask);
		static int WrapR(Texture::MASK _Mask);

		static bool HasMipmap(Texture::MASK _Mask);
	};

}