#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace ynm {

	class YNM_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};

}

//Macros
//Core
#define YNM_CORE_ERROR(...) ::ynm::Log::GetCoreLogger()->error(__VA_ARGS__)
#define YNM_CORE_WARN(...) ::ynm::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define YNM_CORE_INFO(...) ::ynm::Log::GetCoreLogger()->info(__VA_ARGS__)
#define YNM_CORE_TRACE(...) ::ynm::Log::GetCoreLogger()->trace(__VA_ARGS__)

//Client
#define YNM_ERROR(...) ::ynm::Log::GetClientLogger()->error(__VA_ARGS__)
#define YNM_WARN(...) ::ynm::Log::GetClientLogger()->warn(__VA_ARGS__)
#define YNM_INFO(...) ::ynm::Log::GetClientLogger()->info(__VA_ARGS__)
#define YNM_TRACE(...) ::ynm::Log::GetClientLogger()->trace(__VA_ARGS__)