#pragma once

#include "IEditorWindow.h"
#include "../GameplaySystem/Scene.h"
#include "../GameplaySystem/GameObject.h"
#include "../GameplaySystem/Field.h"

#include <memory>
#include <unordered_set>

class PropertyWindow : public IEditorWindow
{
public:
	std::shared_ptr<engine::Scene> GetScene() const;
	void SetScene(std::shared_ptr<engine::Scene> scene);

	std::shared_ptr<engine::GameObject> GetGameObject();
	void SetGameObject(std::shared_ptr<engine::GameObject> gameObject);

	PropertyWindow(const mono::mono_assembly& assembly);

	~PropertyWindow() override;

	void Draw() override;

private:
	static const size_t kGameObjectNameMaxSize = 15;

	std::shared_ptr<engine::Scene> scene;
	std::shared_ptr<engine::GameObject> game_object;

	const char** available_components_items;
	size_t avaliable_components_count;

	void** game_objects_pointers;
	char** game_objects_names;
	size_t game_objects_copasity;

	const mono::mono_assembly& assembly;
	std::vector<std::string> components_names;

	void CacheComponentsData();

	void DrawGameObjectFields();

	void DrawComponentFields(std::shared_ptr<engine::Component> component);

	void DrawAddComponentPanel();

	bool DrawFloatField(
		engine::Field property, 
		const std::vector<engine::Object>& attributes);

	bool DrawDoubleField(
		engine::Field property, 
		const std::vector<engine::Object>& attributes);

	bool DrawBoolField(
		engine::Field property, 
		const std::vector<engine::Object>& attributes);

	bool DrawByteField(
		engine::Field property, 
		const std::vector<engine::Object>& attributes);

	bool DrawShortField(
		engine::Field property, 
		const std::vector<engine::Object>& attributes);

	bool DrawIntField(
		engine::Field property, 
		const std::vector<engine::Object>& attributes);

	bool DrawLongField(
		engine::Field property, 
		const std::vector<engine::Object>& attributes);

	bool DrawUByteField(
		engine::Field property, 
		const std::vector<engine::Object>& attributes);

	bool DrawUShortField(
		engine::Field property, 
		const std::vector<engine::Object>& attributes);

	bool DrawUIntField(
		engine::Field property, 
		const std::vector<engine::Object>& attributes);

	bool DrawULongField(
		engine::Field property, 
		const std::vector<engine::Object>& attributes);

	bool DrawVector2Field(
		engine::Field property, 
		const std::vector<engine::Object>& attributes);

	bool DrawVector3Field(
		engine::Field property, 
		const std::vector<engine::Object>& attributes);

	bool DrawVector4Field(
		engine::Field property,
		const std::vector<engine::Object>& attributes);

	bool DrawStringField(
		engine::Field property,
		const std::vector<engine::Object>& attributes);

	bool DrawGameObjectField(
		engine::Field property,
		const std::vector<engine::Object>& attributes);

	void ParseFullName(
		const std::string& fullName, 
		std::string& namespace_out, 
		std::string& name_out);

	void CopyAsNullTerminated(char* destination, const std::string& source);

	void ChangeGameObjectResourcesCopasity(size_t size);

	void FindAvaliableComponents();

	static std::string GetFieldName(
		const engine::Field& property,
		const std::vector<engine::Object>& attributes);

	static bool IsEditableField(
		const engine::Field& property,
		const std::vector<engine::Object>& attributes);

	static bool TryGetSliderConstraints(
		const std::vector<engine::Object>& attributes,
		float& min_out,
		float& max_out);
};
