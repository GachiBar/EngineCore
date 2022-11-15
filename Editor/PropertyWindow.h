#pragma once

#include "../GameplaySystem/Scene.h"
#include "../GameplaySystem/GameObject.h"
#include "../GameplaySystem/Property.h"
#include "../GameplaySystem/Attribute.h"

#include <memory>
#include <unordered_set>

#include "Interfaces/IApplication.h"

class PropertyWindow
{
public:
	engine::Scene* GetScene() const;

	std::shared_ptr<engine::GameObject> GetGameObject();
	void SetGameObject(std::shared_ptr<engine::GameObject> gameObject);

	PropertyWindow(const mono::mono_assembly& assembly);

	~PropertyWindow();

	void draw_imgui();

	IApplication* app;
private:
	static const size_t kGameObjectNameMaxSize = 15;

	std::shared_ptr<engine::GameObject> game_object;

	const char** available_components_items;
	size_t avaliable_components_count;

	void** game_objects_pointers;
	char** game_objects_names;
	size_t game_objects_copasity;

	const mono::mono_assembly& assembly;
	std::vector<std::string> components_names;

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

	void FindAvaliableComponents();

	static std::string GetPropertyName(
		const engine::Property& property,
		const std::vector<engine::Attribute>& attributes);

	static bool IsEditableProperty(
		const engine::Property& property,
		const std::vector<engine::Attribute>& attributes);

	static bool TryGetSliderConstraints(
		const std::vector<engine::Attribute>& attributes,
		float& min_out,
		float& max_out);
};
