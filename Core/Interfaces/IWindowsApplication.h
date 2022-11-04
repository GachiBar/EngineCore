#pragma once
#include <WinUser.h>

#include "IApplication.h"

class IWindowsApplication : public IApplication
{
public:
	~IWindowsApplication() override = default;

	virtual WNDPROC GetWndProc() = 0;
};
