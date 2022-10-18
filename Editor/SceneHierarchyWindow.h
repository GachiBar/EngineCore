#pragma once
#include <string>

namespace engine
{
	class CSharpObject;
}

class SceneHierarchyWindow
{
public:

	void draw_imgui(engine::CSharpObject& scene);

	bool DoTreeNode(engine::CSharpObject& go, int index);
};

