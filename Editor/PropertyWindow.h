#pragma once

#include "../GameplaySystem/GameObject.h"
#include "../GameplaySystem/ComponentProperty.h"

#include <memory>

class PropertyWindow
{
public:
	void draw_imgui(std::shared_ptr<engine::GameObject> gameObject);

private:
	void DrawGameObjectProperties(std::shared_ptr<engine::GameObject> gameObject);
	void DrawComponentProperties(
		std::shared_ptr<engine::GameObject> gameObject, 
		std::shared_ptr<engine::Component> component);

	void DrawFloatProperty(engine::ComponentProperty property);
	void DrawDoubleProperty(engine::ComponentProperty property);
	void DrawBoolProperty(engine::ComponentProperty property);
	void DrawByteProperty(engine::ComponentProperty property);
	void DrawShortProperty(engine::ComponentProperty property);
	void DrawIntProperty(engine::ComponentProperty property);
	void DrawLongProperty(engine::ComponentProperty property);
	void DrawUByteProperty(engine::ComponentProperty property);
	void DrawUShortProperty(engine::ComponentProperty property);
	void DrawUIntProperty(engine::ComponentProperty property);
	void DrawULongProperty(engine::ComponentProperty property);
	void DrawVector2Property(engine::ComponentProperty property);
	void DrawVector3Property(engine::ComponentProperty property);
	void DrawVector4Property(engine::ComponentProperty property);
	void DrawStringProperty(engine::ComponentProperty property);
	void DrawGameObjectProperty(engine::ComponentProperty property);
};

