#pragma once

#include "GameObject.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_object.h"

namespace engine {

class Scene
{
public:
	const mono::mono_object& GetInternal() const;
	size_t Count() const;

	Scene(const mono::mono_assembly& assembly);

	std::shared_ptr<GameObject> CreateGameObject();
	void DeleteGameObject(std::shared_ptr<GameObject> game_object);

	void Initialize();
	void FixedUpdate();
	void Update();
	void Render();
	void Terminate();
	void Invalidate();

	std::shared_ptr<GameObject> operator[](size_t index) const;

	static void CacheMethods(const mono::mono_assembly& assembly);

private:
	const mono::mono_assembly& assembly_;
	mono::mono_object object_;

	static mono::mono_property_invoker* count_;
	static mono::mono_method_invoker* get_item_;

	static mono::mono_method_invoker* initialize_;
	static mono::mono_method_invoker* terminate_;
	static mono::mono_method_invoker* fixed_update_;
	static mono::mono_method_invoker* update_;
	static mono::mono_method_invoker* render_;
	static mono::mono_method_invoker* invalidate_;

	static mono::mono_method_invoker* create_game_object_;
	static mono::mono_method_invoker* delete_game_object_;
};

} // namespace engine
