#pragma once
#include <string>

#include "../Core/libs/Delegates.h"
#include "../Core/libs/loguru/loguru.hpp"
#include <SimpleMath.h>
#include <source_location>

namespace loguru
{
	struct Message;
}

class LogManager final
{
public:
	static LogManager& getInstance();

	LogManager(LogManager const&) = delete;
	void operator=(LogManager const&) = delete;

	void Initialize(int argc, char* argv[]);

	DECLARE_EVENT(LogMessageEvent, LogManager, loguru::Message const&)
		LogMessageEvent LogMessageAdded;

	DECLARE_EVENT(ViewportMessageEvent, LogManager, std::string const&,loguru::Verbosity, std::string const&)
		ViewportMessageEvent ViewportMessageAdded;

	DECLARE_EVENT(RemoveViewportMessageEvent, LogManager, std::string const&)
		RemoveViewportMessageEvent ViewportMessageRemoved;

	void Log(loguru::Verbosity verbosity, const char* format, ...) const;

	DirectX::SimpleMath::Vector4 GetLevelLogColor(loguru::Verbosity InVerbosity) const;

	void Shutdown();

	void SetNextMessageNotBroadcastLogViewport();
	bool GetNextMessageNotBroadcastLogViewport();

	void SetNextMessageNotBroadcastLog();
	bool GetNextMessageNotBroadcastLog();

	~LogManager();

	void OnRemoveViewportPrint(std::string const& guid);
	void OnViewportPrint(std::string const& message,loguru::Verbosity verbosity, std::string const& guid);
	void OnLogPrint(loguru::Message const& message);

private:
	std::atomic_bool bNotBroadcastNextMessageToLog = false;
	std::atomic_bool bNotBroadcastNextMessageToViewport = false;

	LogManager();

	bool bIsInited = false;
	std::string format_string(const char* format, va_list args) const;
	static void OnLogCallback(void* user_data, loguru::Message const& message);
};

