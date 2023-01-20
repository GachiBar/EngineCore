#pragma once

#include "Object.h"
#include "Runtime.h"

namespace engine {

class GameObject;

class Scene : public Object {
public:
	using Object::Object;

	size_t Count() const;	

	Scene();
	Scene(const Object& other);
	Scene(Object&& other) noexcept;

	std::shared_ptr<GameObject> CreateGameObject();
	void DeleteGameObject(std::shared_ptr<GameObject> game_object);

	void Initialize();
	void FixedUpdate();
	void UpdateAI();
	void Update();
	void Render();
	void EditorRender();
	void RenderGUI();
	void Terminate();
	void Invalidate();

	std::string Serialize();
	void Deserialize(const std::string& data);

	std::shared_ptr<GameObject> FindGameObjectById(uint32_t id);

	std::shared_ptr<GameObject> operator[](size_t index) const;

	static void CacheMethods(const Runtime& runtime);

private:
	static Property* count_;
	static Method* get_item_;

	static Method* initialize_;
	static Method* terminate_;
	static Method* fixed_update_;
	static Method* update_ai_;
	static Method* update_;
	static Method* render_;
	static Method* editor_render_;
	static Method* render_gui_;
	static Method* invalidate_;
	static Method* serialize_;
	static Method* deserialize_;
	static Method* find_game_object_by_id_;

	static Method* create_game_object_;
	static Method* delete_game_object_;

	static Method* setup_imgui_;
};

} // namespace engine
