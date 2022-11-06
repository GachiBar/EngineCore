#pragma once
#include <memory>

class FGenericWindow;

class IApplication
{
public:

	virtual ~IApplication() = default;

	virtual std::shared_ptr<FGenericWindow> GetMainWindow() = 0;
};