#pragma once

#include "../GameplaySystem/GameObject.h"

#include <memory>

class PropertyWindow
{
public:
	void draw_imgui(std::shared_ptr<engine::GameObject> go);
};

