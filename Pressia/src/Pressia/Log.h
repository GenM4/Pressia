#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Pressia {
	class PRESSIA_API Log {
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() {
			return s_CoreLogger;
		}
		inline static Ref<spdlog::logger>& GetClientLogger() {
			return s_ClientLogger;
		}

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
} // namespace Pressia

// Core Log Macros
#define PS_CORE_FATAL(...) ::Pressia::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define PS_CORE_ERROR(...) ::Pressia::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PS_CORE_WARN(...) ::Pressia::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PS_CORE_INFO(...) ::Pressia::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PS_CORE_TRACE(...) ::Pressia::Log::GetCoreLogger()->trace(__VA_ARGS__)

// Client Log Macros
#define PS_FATAL(...) ::Pressia::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define PS_ERROR(...) ::Pressia::Log::GetClientLogger()->error(__VA_ARGS__)
#define PS_WARN(...) ::Pressia::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PS_INFO(...) ::Pressia::Log::GetClientLogger()->info(__VA_ARGS__)
#define PS_TRACE(...) ::Pressia::Log::GetClientLogger()->trace(__VA_ARGS__)