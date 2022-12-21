#pragma once
#include "IEditorWindow.h"

class AIEditorWindow : public IEditorWindow
{
public:
    AIEditorWindow() = default;
    void Draw() override;
};
