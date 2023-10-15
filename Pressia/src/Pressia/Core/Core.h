#pragma once
#include "Pressia/Core/CoreConfig.h"

#include <memory>

#ifdef PS_PLATFORM_WINDOWS
#if PS_DYNAMIC_LINK
#ifdef PS_BUILD_DLL
#define PRESSIA_API __declspec(dllexport)
#else
#define PRESSIA_API __declspec(dllimport)
#endif // PS_BUILD_DLL
#else
#define PRESSIA_API
#endif
#else
#error Only Windows support for now!
#endif // PS_PLATFORM_WINDOWS

#ifdef PS_ENABLE_ASSERTS
#define PS_ASSERT(x, ...) {if(!(x)) {PS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define PS_CORE_ASSERT(x, ...) {if(!(x)) {PS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define PS_ASSERT(x, ...)
#define PS_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define PS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Pressia {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}