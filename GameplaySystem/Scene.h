#pragma once

#include "Object.h"
#include "Runtime.h"

namespace engine {

class GameObject;

class Scene : public Object {
public:
	size_t Count() const;

	Scene(const Runtime& runtime);

	std::shared_ptr<GameObject> CreateGameObject();
	void DeleteGameObject(std::shared_ptr<GameObject> game_object);

	void Initialize();
	void FixedUpdate();
	void Update();
	void Render();
	void Terminate();
	void Invalidate();

	std::string Serialize();
	void Deserialize(const std::string& data);

	std::shared_ptr<GameObject> operator[](size_t index) const;

	static void CacheMethods(const Runtime& runtime);

private:
	const const Runtime& runtime_;

	static Property* count_;
	static Method* get_item_;

	static Method* initialize_;
	static Method* terminate_;
	static Method* fixed_update_;
	static Method* update_;
	static Method* render_;
	static Method* invalidate_;
	static Method* serialize_;
	static Method* deserialize_;

	static Method* create_game_object_;
	static Method* delete_game_object_;
};

} // namespace engine
