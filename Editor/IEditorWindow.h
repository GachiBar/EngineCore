#pragma once

class IEditorWindow {
public:
	virtual ~IEditorWindow() {}
	virtual void Draw() = 0;
};