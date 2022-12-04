#pragma once

#include "../GameplaySystem/Scene.h"
#include "../GameplaySystem/GameObject.h"

class ObjectDrawer 
{
public:
	std::shared_ptr<engine::Scene> GetScene() const;
	void SetScene(std::shared_ptr<engine::Scene> scene);

	ObjectDrawer();
	~ObjectDrawer();

	bool DrawObject(engine::Object& object);

private:
	static const size_t kGameObjectNameMaxSize = 15;

	std::shared_ptr<engine::Scene> scene;

	void** game_objects_pointers;
	char** game_objects_names;
	size_t game_objects_capacity;

	bool DrawFloatField(const engine::Object& object, engine::Field field);
	bool DrawDoubleField(const engine::Object& object, engine::Field field);
	bool DrawBoolField(const engine::Object& object, engine::Field field);
	bool DrawByteField(const engine::Object& object, engine::Field field);
	bool DrawShortField(const engine::Object& object, engine::Field field);
	bool DrawIntField(const engine::Object& object, engine::Field field);
	bool DrawLongField(const engine::Object& object, engine::Field field);
	bool DrawUByteField(const engine::Object& object, engine::Field field);
	bool DrawUShortField(const engine::Object& object, engine::Field field);
	bool DrawUIntField(const engine::Object& object, engine::Field field);
	bool DrawULongField(const engine::Object& object, engine::Field field);
	bool DrawVector2Field(const engine::Object& object, engine::Field field);
	bool DrawVector3Field(const engine::Object& object, engine::Field field);
	bool DrawVector4Field(const engine::Object& object, engine::Field field);
	bool DrawStringField(const engine::Object& object, engine::Field field);
	bool DrawGameObjectField(const engine::Object& object, engine::Field field);

	static std::string GetFieldName(
		const engine::Field& field,
		const std::vector<engine::Object>& attributes);

	static bool IsEditableField(
		const engine::Field& field,
		const std::vector<engine::Object>& attributes);

	static bool TryGetSliderConstraints(
		const std::vector<engine::Object>& attributes,
		float& min_out,
		float& max_out);

	void ChangeGameObjectResourcesCapacity(size_t size);
	void CopyAsNullTerminated(char* destination, const std::string& source);
};