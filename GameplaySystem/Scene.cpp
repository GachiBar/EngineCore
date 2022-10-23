#include "pch.h"
#include "Scene.h"
#include "../monowrapper/monopp/mono_assembly.h"

mono::mono_method_invoker* Scene::initialize_ = nullptr;
mono::mono_method_invoker* Scene::terminate_ = nullptr;
mono::mono_method_invoker* Scene::fixed_update_ = nullptr;
mono::mono_method_invoker* Scene::update_ = nullptr;
mono::mono_method_invoker* Scene::render_ = nullptr;
mono::mono_method_invoker* Scene::invalidate_ = nullptr;
mono::mono_method_invoker* Scene::create_game_object_ = nullptr;
mono::mono_method_invoker* Scene::delete_game_object_ = nullptr;

const mono::mono_object& Scene::GetInternal() {
    return object_;
}

Scene::Scene(const mono::mono_assembly& assembly)
    : assembly_(assembly)
    , object_(assembly.get_type("GameplayCore", "Scene").new_instance())
{}

std::shared_ptr<GameObject> Scene::CreateGameObject() {
    mono::mono_object object(create_game_object_->invoke(object_));
    Invalidate();
    return std::shared_ptr<GameObject>(new GameObject(assembly_, std::move(object)));
}

void Scene::DeleteGameObject(std::shared_ptr<GameObject> game_object) {
    void* params[1] = {game_object->GetInternal().get_internal_ptr()};
    delete_game_object_->invoke(object_, params);
    Invalidate();
    game_object.reset();
}

void Scene::Initialize() {
    assert(initialize_ != nullptr && "Scene methods are not cached.");
    initialize_->invoke(object_);
}

void Scene::FixedUpdate() {
    assert(fixed_update_ != nullptr && "Scene methods are not cached.");
    fixed_update_->invoke(object_);
}

void Scene::Update() {
    assert(update_ != nullptr && "Scene methods are not cached.");
    update_->invoke(object_);
}

void Scene::Render() {
    assert(render_ != nullptr && "Scene methods are not cached.");
    render_->invoke(object_);
}

void Scene::Terminate() {
    assert(terminate_ != nullptr && "Scene methods are not cached.");
    terminate_->invoke(object_);
}

void Scene::Invalidate() {
    assert(invalidate_ != nullptr && "Scene methods are not cached.");
    invalidate_->invoke(object_);
}

void Scene::CacheMethods(const mono::mono_assembly& assembly) {
    mono::mono_type type = assembly.get_type("GameplayCore", "Scene");
    
    mono::mono_method initialize_method(type, "Initialize", 0);
    initialize_ = new mono::mono_method_invoker(initialize_method);

    mono::mono_method terminate_method(type, "Terminate", 0);
    terminate_ = new mono::mono_method_invoker(terminate_method);

    mono::mono_method fixed_update_method(type, "FixedUpdate", 0);
    fixed_update_ = new mono::mono_method_invoker(fixed_update_method);

    mono::mono_method update_method(type, "Update", 0);
    update_ = new mono::mono_method_invoker(update_method);

    mono::mono_method render_method(type, "Render", 0);
    render_ = new mono::mono_method_invoker(render_method);

    mono::mono_method invalidate_method(type, "Invalidate", 0);
    invalidate_ = new mono::mono_method_invoker(invalidate_method);

    mono::mono_method create_game_object_method(type, "CreateGameObject", 0);
    create_game_object_ = new mono::mono_method_invoker(create_game_object_method);

    mono::mono_method delete_game_object_method(type, "DeleteGameObject", 1);
    delete_game_object_ = new mono::mono_method_invoker(delete_game_object_method);
}
