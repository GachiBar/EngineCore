#include "pch.h"
#include "Scene.h"
#include "../monowrapper/monopp/mono_assembly.h"

mono::mono_method_invoker* Scene::initialize_;
mono::mono_method_invoker* Scene::terminate_;
mono::mono_method_invoker* Scene::fixed_update_;
mono::mono_method_invoker* Scene::update_;
mono::mono_method_invoker* Scene::render_;
mono::mono_method_invoker* Scene::invalidate_;
mono::mono_method_invoker* Scene::create_game_object_;
mono::mono_method_invoker* Scene::delete_game_object_;


Scene::Scene(const mono::mono_assembly& assembly)
{
    object_reference = new mono::mono_object(assembly.get_type("GameplayCore", "Scene").new_instance());
}

GameObject* Scene::CreateGameObject() const
{
    MonoObject* object = create_game_object_->invoke(*object_reference);
    const auto game_object_ref = new mono::mono_object(object);
    return new GameObject(game_object_ref);
}

void Scene::DeleteGameObject(GameObject* game_object)
{
    void* params[1] = {game_object->GetInternalPtr().get_internal_ptr()};
    delete_game_object_->invoke(*object_reference, params);
    delete game_object;
}

void Scene::Initialize()
{
    initialize_->invoke(*object_reference);
}

void Scene::FixedUpdate()
{
    fixed_update_->invoke(*object_reference);
}

void Scene::Update()
{
    update_->invoke(*object_reference);
}

void Scene::Render()
{
    render_->invoke(*object_reference);
}

void Scene::Terminate()
{
    terminate_->invoke(*object_reference);
}

void Scene::Invalidate()
{
    invalidate_->invoke(*object_reference);
}

const mono::mono_object& Scene::GetInternalPtr()
{
    return *object_reference;
}

void Scene::CacheMethods(const mono::mono_assembly& assembly)
{
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
