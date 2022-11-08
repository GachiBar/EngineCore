#pragma once
#include "../Definitions.h"

class IRender
{
public:

	virtual ~IRender() = default;

	virtual void ResizeViewport(int32 InWidth, int32 InHeight) = 0;

	virtual void ResizeBackBuffer(int32 InWidth, int32 InHeight) = 0;
};