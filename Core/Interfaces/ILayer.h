#pragma once
#include "../Enums/EditorInputMode.h"


class ILayer
{
public:
	virtual ~ILayer() = default;

	virtual void OnAttach() = 0;
	virtual void OnDetach() = 0;
	virtual void OnUpdate(float const dt) = 0;
	virtual void OnGuiRender() = 0;
	virtual void OnPostRender() = 0;

	virtual EEditorInputMode::Type GetCurrentInputMode() const = 0;
}; 