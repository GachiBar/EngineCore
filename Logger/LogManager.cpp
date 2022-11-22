#include "LogManager.h"

#include <cstdarg>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "libs/loguru/loguru.hpp"
#include <source_location>
#include <stacktrace>

#define FORMAT_LOG_MESSAGE_IMPLEMENTATION	std::va_list args;\
											va_start(args, format);\
											auto res = format_string(format, args);\
											va_end(args);

LogManager& LogManager::getInstance()
{
	static LogManager instance;

	// Instantiated on first use.
	return instance;
}

LogManager::LogManager()
{
}

void LogManager::Initialize(int argc, char* argv[])
{
	if(!bIsInited)
	{
		bIsInited = true;
		loguru::init(argc, argv);

		loguru::g_preamble_file = false;
		// Put every log message in "everything.log":
		loguru::add_file("everything.log", loguru::Append, loguru::Verbosity_MAX);

		// Only log INFO, WARNING, ERROR and FATAL to "latest_readable.log":
		loguru::add_file("latest_readable.log", loguru::Truncate, loguru::Verbosity_INFO);
		loguru::add_callback("log_callback", LogManager::OnLogCallback, this, loguru::Verbosity_INFO);
	}
	
	/*
	int a = 0;

	std::string str = "SOME_STRING";
	Log("Hello %f and %u and %s", 4.56,30,str.c_str());
	LogWarning("Hello %f and %u and %s", 4.56, 30, str.c_str());
	LogError("Hello %f and %u and %s", 4.56, 30, str.c_str());
	// Only show most relevant things on stderr:
	//loguru::g_stderr_verbosity = 1;

	LOG_SCOPE_F(INFO, "Will indent all log messages within this scope.");
	
	LOG_F(INFO, "I'm hungry for some %.3f!", 3.14159);
	LOG_F(2, "Will only show if verbosity is 2 or higher");
	LOG_F(ERROR, "hello");
	//VLOG_F(get_log_level(), "Use vlog for dynamic log level (integer in the range 0-9, inclusive)");
	//LOG_IF_F(ERROR, badness, "Will only show if badness happens");
	//auto fp = fopen("hello", "r");
	//CHECK_F(fp != nullptr, "Failed to open file '%s'", "hello");
	CHECK_GT_F(10, 0); // Will print the value of `length` on failure.
	CHECK_EQ_F(a, 0, "You can also supply a custom message, like to print something: %d", a + a);

	// Each function also comes with a version prefixed with D for Debug:
	DCHECK_F(a == 0); // Only checked #if !NDEBUG
	DLOG_F(INFO, "Only written in debug-builds");
	*/
}

void LogManager::Log(const char* format, ...) const
{
	FORMAT_LOG_MESSAGE_IMPLEMENTATION
	LOG_F(INFO, "%s", res.c_str());
}

void LogManager::LogWarning(const char* format, ...) const
{
	FORMAT_LOG_MESSAGE_IMPLEMENTATION
	LOG_F(WARNING, "%s", res.c_str());
}

void LogManager::LogError(const char* format, ...) const
{
	FORMAT_LOG_MESSAGE_IMPLEMENTATION
	LOG_F(ERROR, "%s", res.c_str());
}

DirectX::SimpleMath::Vector4 LogManager::GetLevelLogColor(loguru::Verbosity InVerbosity) const
{
	switch (InVerbosity)
	{
	case loguru::Verbosity_ERROR:
	case loguru::Verbosity_FATAL:
		return { 1.f,0.f,0.f,1.f };

	case loguru::Verbosity_WARNING:
		return { 1.f,1.f,0.f,1.f };
	default: 
		return DirectX::SimpleMath::Vector4::One;
	}
}

void LogManager::Shutdown()
{
	LogMessageAdded.RemoveAll();
	ViewportMessageAdded.RemoveAll();
	loguru::shutdown();
}

void LogManager::SetNextMessageNotBroadcastLogViewport()
{
	bNotBroadcastNextMessageToLog = true;
}

bool LogManager::GetNextMessageNotBroadcastLogViewport()
{
	return bNotBroadcastNextMessageToLog;
}

void LogManager::SetNextMessageNotBroadcastLog()
{
	bNotBroadcastNextMessageToLog = true;
}

bool LogManager::GetNextMessageNotBroadcastLog()
{
	return bNotBroadcastNextMessageToLog;
}

LogManager::~LogManager()
{
	Shutdown();
}

std::string LogManager::format_string(const char* format, va_list args) const
{
	auto temp = std::vector<char>{};
	auto length = std::size_t{};
	while (temp.size() <= length)
	{
		temp.resize(length + 1);
		const auto status = std::vsnprintf(temp.data(), temp.size(), format, args);

		CHECK_GE_F(status, 0);

		length = static_cast<std::size_t>(status);
	}
	return std::string{ temp.data(), length };
}

void LogManager::OnLogCallback(void* user_data, loguru::Message const& message)
{
	const auto log_manager_inst = static_cast<LogManager*>(user_data);

	if(log_manager_inst && !log_manager_inst->GetNextMessageNotBroadcastLog())
		log_manager_inst->LogMessageAdded.Broadcast(message);
	if (log_manager_inst && log_manager_inst->GetNextMessageNotBroadcastLog())
		log_manager_inst->bNotBroadcastNextMessageToLog = false;

	if (log_manager_inst && !log_manager_inst->GetNextMessageNotBroadcastLogViewport())
		log_manager_inst->ViewportMessageAdded.Broadcast(message);
	if (log_manager_inst && log_manager_inst->GetNextMessageNotBroadcastLogViewport())
		log_manager_inst->bNotBroadcastNextMessageToViewport = false;
}

#undef FORMAT_LOG_MESSAGE_IMPLEMENTATION