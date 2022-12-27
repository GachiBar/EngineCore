#pragma once

#include <filesystem>

#include "../GameplaySystem/Scene.h"
#include "../GameplaySystem/GameObject.h"
#include "Resources/FileType.h"

class ObjectDrawer 
{
private:
	struct resources_cache
	{
	public:
		resources_cache();
		~resources_cache();

		void update(std::filesystem::path basepath, FileType type);
		std::string get_name(int index) const;
		std::shared_ptr<engine::Object> get_pointer(int index);
		int get_index(const engine::Object& resource);

		int size() const;

		const char** get_names_pointer() {return resource_names.data();}

	private:
		std::vector<std::pair<std::filesystem::path, std::shared_ptr<engine::Object>>> files_path;
		std::vector<const char*> resource_names;
	};
	
public:
	std::shared_ptr<engine::Scene> GetScene() const;
	void SetScene(std::shared_ptr<engine::Scene> scene);

	ObjectDrawer();
	~ObjectDrawer();

	bool DrawObject(engine::Object& object, std::vector<std::string>& modifiedFields);

private:
	static const size_t kGameObjectNameMaxSize = 15;

	std::shared_ptr<engine::Scene> scene;

	void** game_objects_pointers;
	char** game_objects_names;
	size_t game_objects_capacity;

	resources_cache cache_;

	bool DrawFloatField(const engine::Object& object, engine::Field& field);
	bool DrawDoubleField(const engine::Object& object, engine::Field& field);
	bool DrawBoolField(const engine::Object& object, engine::Field& field);
	bool DrawByteField(const engine::Object& object, engine::Field& field);
	bool DrawShortField(const engine::Object& object, engine::Field& field);
	bool DrawIntField(const engine::Object& object, engine::Field& field);
	bool DrawLongField(const engine::Object& object, engine::Field& field);
	bool DrawUByteField(const engine::Object& object, engine::Field& field);
	bool DrawUShortField(const engine::Object& object, engine::Field& field);
	bool DrawUIntField(const engine::Object& object, engine::Field& field);
	bool DrawULongField(const engine::Object& object, engine::Field& field);
	bool DrawVector2Field(const engine::Object& object, engine::Field& field);
	bool DrawVector3Field(const engine::Object& object, engine::Field& field);
	bool DrawVector4Field(const engine::Object& object, engine::Field& field);
	bool DrawStringField(const engine::Object& object, engine::Field& field);
	bool DrawGameObjectField(const engine::Object& object, engine::Field& field);
	bool DrawResourceField(const engine::Object& object, engine::Field& field);
	bool DrawColor3Field(const engine::Object& object, engine::Field& field);
	bool DrawColor4Field(const engine::Object& object, engine::Field& field);

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
	static void CopyAsNullTerminated(char* destination, const std::string& source);	
};
