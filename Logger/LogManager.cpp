#include "LogManager.h"

#include <cstdarg>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "libs/loguru/loguru.hpp"
#include <source_location>
//#include <stacktrace>

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
}

void LogManager::Log(loguru::Verbosity verbosity, const char* format, ...) const
{
	std::va_list args;
	va_start(args, format);
	const auto res = format_string(format, args);
	va_end(args);

	VLOG_F(verbosity, res.c_str());
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

void LogManager::OnRemoveViewportPrint(std::string const& guid)
{
	ViewportMessageRemoved.Broadcast(guid);
}

void LogManager::OnViewportPrint(std::string const& message, loguru::Verbosity verbosity, std::string const& guid)
{
	if (!GetNextMessageNotBroadcastLogViewport())
		ViewportMessageAdded.Broadcast(message,verbosity, guid);
	else
		bNotBroadcastNextMessageToViewport = false;
}

void LogManager::OnLogPrint(loguru::Message const& message)
{
	if (!GetNextMessageNotBroadcastLog())
		LogMessageAdded.Broadcast(message);
	else
		bNotBroadcastNextMessageToLog = false;
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

	if (log_manager_inst)
	{
		log_manager_inst->OnLogPrint(message);
	}
}

#undef FORMAT_LOG_MESSAGE_IMPLEMENTATION