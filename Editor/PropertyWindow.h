#pragma once

#include "../GameplaySystem/Scene.h"
#include "../GameplaySystem/GameObject.h"
#include "../GameplaySystem/ComponentProperty.h"

#include <memory>
#include <unordered_set>

struct ComponentData
{
	const std::string NameSpace;
	const std::string Name;
	const std::string FullName;

	ComponentData(std::string nameSpace, std::string name)
		: NameSpace(std::move(nameSpace))
		, Name(std::move(name))
		, FullName(NameSpace + "." + Name)
	{}

	friend bool operator== (const ComponentData& lhs, const ComponentData& rhs);
};

template<>
struct std::hash<ComponentData>
{
	size_t operator()(const ComponentData& componentData) const noexcept
	{
		size_t hash1 = std::hash<std::string>{}(componentData.NameSpace);
		size_t hash2 = std::hash<std::string>{}(componentData.Name);
		return hash1 ^ (hash2 << 1);
	}
};

class PropertyWindow
{
public:
	PropertyWindow(const mono::mono_assembly& assembly);
	~PropertyWindow();
	void draw_imgui(
		std::shared_ptr<engine::Scene> scene, 
		std::shared_ptr<engine::GameObject> gameObject);

private:	
	static const size_t kGameObjectNameMaxSize = 15;

	const char** available_components_items;
	void** game_objects_pointers;
	char** game_objects_names;
	size_t game_objects_copasity;

	const mono::mono_assembly& assembly;
	std::vector<ComponentData> components_names;
	std::unordered_set<ComponentData> added_components;	

	void CacheComponentsData();
	void DrawGameObjectProperties(std::shared_ptr<engine::GameObject> gameObject);
	void DrawComponentProperties(
		std::shared_ptr<engine::Scene> scene,
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
	void DrawGameObjectProperty(
		std::shared_ptr<engine::Scene> scene,
		std::shared_ptr<engine::GameObject> gameObject,
		engine::ComponentProperty property);

	void ParseFullName(
		const std::string& fullName, 
		std::string& namespace_out, 
		std::string& name_out);
	void CopyAsNullTerminated(char* destination, const std::string& source);
	void ChangeGameObjectResourcesCopasity(size_t size);
};