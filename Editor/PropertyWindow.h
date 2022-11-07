#pragma once

#include "../GameplaySystem/Scene.h"
#include "../GameplaySystem/GameObject.h"
#include "../GameplaySystem/Property.h"
#include "../GameplaySystem/Attribute.h"

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
	std::shared_ptr<engine::Scene> GetScene();
	void SetScene(std::shared_ptr<engine::Scene> scene);

	std::shared_ptr<engine::GameObject> GetGameObject();
	void SetGameObject(std::shared_ptr<engine::GameObject> gameObject);

	PropertyWindow(const mono::mono_assembly& assembly);

	~PropertyWindow();

	void draw_imgui();

private:	
	static const size_t kGameObjectNameMaxSize = 15;

	std::shared_ptr<engine::Scene> scene;
	std::shared_ptr<engine::GameObject> game_object;

	const char** available_components_items;
	void** game_objects_pointers;
	char** game_objects_names;
	size_t game_objects_copasity;

	const mono::mono_assembly& assembly;
	std::vector<ComponentData> components_names;
	std::unordered_set<ComponentData> added_components;	

	void CacheComponentsData();

	void DrawGameObjectProperties();

	void DrawComponentProperties(std::shared_ptr<engine::Component> component);

	void DrawAddComponentPanel();

	void DrawFloatProperty(
		engine::Property property, 
		const std::vector<engine::Attribute>& attributes);

	void DrawDoubleProperty(
		engine::Property property, 
		const std::vector<engine::Attribute>& attributes);

	void DrawBoolProperty(
		engine::Property property, 
		const std::vector<engine::Attribute>& attributes);

	void DrawByteProperty(
		engine::Property property, 
		const std::vector<engine::Attribute>& attributes);

	void DrawShortProperty(
		engine::Property property, 
		const std::vector<engine::Attribute>& attributes);

	void DrawIntProperty(
		engine::Property property, 
		const std::vector<engine::Attribute>& attributes);

	void DrawLongProperty(
		engine::Property property, 
		const std::vector<engine::Attribute>& attributes);

	void DrawUByteProperty(
		engine::Property property, 
		const std::vector<engine::Attribute>& attributes);

	void DrawUShortProperty(
		engine::Property property, 
		const std::vector<engine::Attribute>& attributes);

	void DrawUIntProperty(
		engine::Property property, 
		const std::vector<engine::Attribute>& attributes);

	void DrawULongProperty(
		engine::Property property, 
		const std::vector<engine::Attribute>& attributes);

	void DrawVector2Property(
		engine::Property property, 
		const std::vector<engine::Attribute>& attributes);

	void DrawVector3Property(
		engine::Property property, 
		const std::vector<engine::Attribute>& attributes);

	void DrawVector4Property(
		engine::Property property,
		const std::vector<engine::Attribute>& attributes);

	void DrawStringProperty(
		engine::Property property,
		const std::vector<engine::Attribute>& attributes);

	void DrawGameObjectProperty(
		engine::Property property,
		const std::vector<engine::Attribute>& attributes);

	void ParseFullName(
		const std::string& fullName, 
		std::string& namespace_out, 
		std::string& name_out);
	void CopyAsNullTerminated(char* destination, const std::string& source);

	void ChangeGameObjectResourcesCopasity(size_t size);

	static std::string GetPropertyName(
		const engine::Property& property,
		const std::vector<engine::Attribute>& attributes);

	static bool IsEditableProperty(
		const engine::Property& property,
		const std::vector<engine::Attribute>& attributes);

	static bool IsSliderProperty(
		const std::vector<engine::Attribute>& attributes,
		float& min_out,
		float& max_out);
};