#pragma once
#include <string>

#include "../Core/libs/Delegates.h"
#include "../Core/libs/loguru/loguru.hpp"

namespace loguru
{
	struct Message;
}

class LogManager final
{
public:
	void Initialize(int argc, char* argv[]);

	DECLARE_EVENT(LogMessageEvent, LogManager, std::string const &, loguru::Verbosity)
		LogMessageEvent LogMessageAdded;
	
	void Shutdown();
private:
	static void OnLogCallback(void* user_data, loguru::Message const& message);
};

