#include "LogManager.h"

#include <memory>
#include <string>
#include <vector>
#include "libs/loguru/loguru.hpp"

void LogManager::Initialize(int argc, char* argv[])
{
	int a = 0;
	loguru::init(argc, argv);

	// Put every log message in "everything.log":
	loguru::add_file("everything.log", loguru::Append, loguru::Verbosity_MAX);

	// Only log INFO, WARNING, ERROR and FATAL to "latest_readable.log":
	loguru::add_file("latest_readable.log", loguru::Truncate, loguru::Verbosity_INFO);
	loguru::add_callback("log_callback", LogManager::OnLogCallback, nullptr, loguru::Verbosity_INFO);
	
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
}

void LogManager::Shutdown()
{
	LogMessageAdded.RemoveAll();
	loguru::shutdown();
}

void LogManager::OnLogCallback(void* user_data, loguru::Message const& message)
{
	if(const auto log_manager_inst = static_cast<LogManager*>(user_data))
		log_manager_inst->LogMessageAdded.Broadcast(std::string(message.preamble) + std::string(message.message),message.verbosity);
}
