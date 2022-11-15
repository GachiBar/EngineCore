#pragma once
#include <memory>

#include "ILayer.h"
#include "../Definitions.h"

class IRender;

namespace engine
{
	class Engine;
}

class FGenericWindow;

class IApplication
{
public:

	virtual ~IApplication() = default;

	virtual std::shared_ptr<FGenericWindow> GetMainWindow() = 0;
	virtual ILayer* GetCurrentUpdateLayer() = 0;

	virtual engine::Engine* GetEngine() const = 0;

	virtual void ResizeBackBuffer(int32 InWidth, int32 InHeight) = 0;
	virtual void ResizeViewport(int32 InWidth, int32 InHeight) = 0;

	virtual EEditorInputMode::Type GetCurrentInputMode() const = 0;
};