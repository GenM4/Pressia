#pragma once

#include <memory>

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

	template<typename T>
	using Observe = std::weak_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateObserver(Args&& ... args) {
		return std::make_weak<T>(std::forward<Args>(args)...);
	}
}