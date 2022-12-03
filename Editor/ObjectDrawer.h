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
	size_t game_objects_copasity;

	bool DrawFloatField(engine::Field field);
	bool DrawDoubleField(engine::Field field);
	bool DrawBoolField(engine::Field field);
	bool DrawByteField(engine::Field field);
	bool DrawShortField(engine::Field field);
	bool DrawIntField(engine::Field field);
	bool DrawLongField(engine::Field field);
	bool DrawUByteField(engine::Field field);
	bool DrawUShortField(engine::Field field);
	bool DrawUIntField(engine::Field field);
	bool DrawULongField(engine::Field field);
	bool DrawVector2Field(engine::Field field);
	bool DrawVector3Field(engine::Field field);
	bool DrawVector4Field(engine::Field field);
	bool DrawStringField(engine::Field field);
	bool DrawGameObjectField(engine::Field field);

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

	void ChangeGameObjectResourcesCopasity(size_t size);
	void CopyAsNullTerminated(char* destination, const std::string& source);
};