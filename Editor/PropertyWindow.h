#pragma once

#include "../GameplaySystem/GameObject.h"
#include "../GameplaySystem/ComponentProperty.h"

#include <memory>
#include <unordered_set>

struct ComponentData
{
	std::string NameSpace;
	std::string Name;
	std::string FullName;

	bool operator== (const ComponentData& other);
	bool operator< (const ComponentData& other);

	friend bool operator== (const ComponentData& lhs, const ComponentData& rhs);
};

template<>
struct std::hash<ComponentData>
{
	std::size_t operator()(const ComponentData& componentData) const noexcept
	{
		return std::hash<std::string>{}(componentData.FullName);
	}
};

class PropertyWindow
{
public:
	PropertyWindow(const mono::mono_assembly& assembly);
	~PropertyWindow() = default;
	void draw_imgui(std::shared_ptr<engine::GameObject> gameObject);

private:
	const mono::mono_assembly& assembly;
	std::vector<ComponentData> components_names;
	std::unordered_set<ComponentData> available_components;

	void DrawGameObjectProperties(std::shared_ptr<engine::GameObject> gameObject);
	void DrawComponentProperties(
		std::shared_ptr<engine::GameObject> gameObject, 
		std::shared_ptr<engine::Component> component);
	void DrawAddComponentPanel(std::shared_ptr<engine::GameObject> gameObject);

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