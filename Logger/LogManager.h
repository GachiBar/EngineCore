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

	DECLARE_EVENT(LogMessageEvent, LogManager, std::string const &, loguru::Verbosity)
		LogMessageEvent LogMessageAdded;

	void Log(const char* format, ...) const;
	void LogWarn(const char* format, ...) const;
	void LogError(const char* format, ...) const;

	DirectX::SimpleMath::Vector4 GetLevelLogColor(loguru::Verbosity InVerbosity) const;

	void Shutdown();

	~LogManager();
private:
	LogManager();

	bool bIsInited = false;
	std::string format_string(const char* format, va_list args) const;
	static void OnLogCallback(void* user_data, loguru::Message const& message);
};

