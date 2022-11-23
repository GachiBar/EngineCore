#pragma once

class IEditorWindow {
public:
	virtual ~IEditorWindow() = default;
	virtual void Draw() = 0;
};