#pragma once

#include <memory>

#ifdef DEBUG
	#if defined(WIN32)
		#define DEBUGBREAK() __debugbreak()
	#elif defined(LINUX)
		#include <signal.h>
		#define DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "PLATFORM DOESN'T SUPPORT DEBUGBREAK!"
	#endif
#else
	#define DEBUGBREAK()
#endif

#define BIT(x) (1 << x)

namespace LM
{

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename F>
	constexpr Ref<T> StaticRefCast(Ref<F> _From)
	{
		return std::static_pointer_cast<T>(_From);
	}

}