#pragma once

#include <vector>

#include <glm.hpp>

#include "Vega/Core/Base.h"
#include "Vega/Core/Application.h"

namespace LM
{
	namespace FrameBufferColorMASK
	{
		enum FrameBufferColorMASK : uint32_t
		{
			NONE = 0,
			HDR = BIT(0),
		};
	}

	namespace FrameBufferMASK
	{
		enum FrameBufferMASK : uint32_t
		{
			NONE = 0,
			DEPTH = BIT(0),
			STENCIL = BIT(1)
		};
	}
	struct FrameBufferProps
	{
		static FrameBufferProps FromWindow(const std::initializer_list<FrameBufferColorMASK::FrameBufferColorMASK> _ColorMask = {}, FrameBufferMASK::FrameBufferMASK _FBMask = FrameBufferMASK::NONE)
		{ 
			return 
			{
				Application::Get()->GetWindow()->GetWidth(),
				Application::Get()->GetWindow()->GetHeight(),
				Application::Get()->GetWindow()->GetMSAA(),
				_ColorMask,
				_FBMask
			};
		}

		uint32_t Width  = 128;
		uint32_t Height = 128;
		WindowMSAA MSAA = WindowMSAA::None;
		std::vector<FrameBufferColorMASK::FrameBufferColorMASK> FBColorMask;
		FrameBufferMASK::FrameBufferMASK FBMask = FrameBufferMASK::NONE;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() { }

		virtual void Bind()		const = 0;
		virtual void Unbind()	const = 0;

		virtual void Clear(glm::vec4 _Color)	const = 0;
		
		virtual void* GetId() const = 0;
		virtual uint32_t GetTextureCount() const = 0;
		virtual void* GetTextureId(uint32_t _Id = 0) const = 0;
		virtual void* GetTextureHandle(uint32_t _Id = 0) const = 0;	

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Resize(uint32_t _Width, uint32_t _Height) = 0;

		virtual void BindColorTexture(uint32_t _Id, uint32_t _SlotId) = 0;

		static Ref<FrameBuffer> Create(const FrameBufferProps& _Props = FrameBufferProps());
	};

};