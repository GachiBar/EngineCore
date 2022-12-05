#include "pch.h"
#include "Scene.h"
#include "GameObject.h"

namespace engine {

Property* Scene::count_ = nullptr;
Method* Scene::get_item_ = nullptr;

Method* Scene::initialize_ = nullptr;
Method* Scene::terminate_ = nullptr;
Method* Scene::fixed_update_ = nullptr;
Method* Scene::update_ = nullptr;
Method* Scene::render_ = nullptr;
Method* Scene::invalidate_ = nullptr;
Method* Scene::serialize_ = nullptr;
Method* Scene::deserialize_ = nullptr;

Method* Scene::create_game_object_ = nullptr;
Method* Scene::delete_game_object_ = nullptr;

const char kIsNotCachedErrorMessage[] = "Scene methods are not cached.";

size_t Scene::Count() const {
    assert(count_ != nullptr && kIsNotCachedErrorMessage);

    auto result = count_->GetValue(*this).value();
    int count = result.Unbox<int>();
    return static_cast<size_t>(count);
}

Scene::Scene(const Runtime& runtime)
    : Object(runtime.GetType(Types::kScene).Instantiate())
    , runtime_(runtime)
{}

std::shared_ptr<GameObject> Scene::CreateGameObject() {
    assert(create_game_object_ != nullptr && kIsNotCachedErrorMessage);

    auto game_object = create_game_object_->Invoke(*this).value();
    return std::make_shared<GameObject>(runtime_, game_object.GetInternal());
}

void Scene::DeleteGameObject(std::shared_ptr<GameObject> game_object) {
    assert(delete_game_object_ != nullptr && kIsNotCachedErrorMessage);

    void* params[1];
    params[0] = game_object->GetInternal().get_internal_ptr();

    delete_game_object_->Invoke(*this, params);
    game_object.reset();
}

void Scene::Initialize() {
    assert(initialize_ != nullptr && kIsNotCachedErrorMessage);

    initialize_->Invoke(*this);
}

void Scene::FixedUpdate() {
    assert(fixed_update_ != nullptr && kIsNotCachedErrorMessage);

    fixed_update_->Invoke(*this);
}

void Scene::Update() {
    assert(update_ != nullptr && kIsNotCachedErrorMessage);

    update_->Invoke(*this);
}

void Scene::Render() {
    assert(render_ != nullptr && kIsNotCachedErrorMessage);

    render_->Invoke(*this);
}

void Scene::Terminate() {
    assert(terminate_ != nullptr && kIsNotCachedErrorMessage);

    terminate_->Invoke(*this);
}

void Scene::Invalidate() {
    assert(invalidate_ != nullptr && kIsNotCachedErrorMessage);

    invalidate_->Invoke(*this);
}

std::string Scene::Serialize()
{
    assert(serialize_ != nullptr && kIsNotCachedErrorMessage);

    auto result = serialize_->Invoke(*this).value();
    mono::mono_string json(result.GetInternal());
    // Call method serialize
    return json.as_utf8();
}

void Scene::Deserialize(const std::string& data)
{
    assert(deserialize_ != nullptr && kIsNotCachedErrorMessage);

    auto& domain = mono::mono_domain::get_current_domain();
    mono::mono_string json(domain, data);

    void* params[1];
    params[0] = json.get_internal_ptr();

    deserialize_->Invoke(*this, params);
    // Call method deserialize
}

std::shared_ptr<GameObject> Scene::operator[](size_t index) const {
    assert(get_item_ != nullptr && kIsNotCachedErrorMessage);

    void* args[1];
    args[0] = &index;
    
    auto game_object = get_item_->Invoke(*this, args).value();
    return std::make_shared<GameObject>(runtime_, game_object.GetInternal());
}

void Scene::CacheMethods(const Runtime& runtime) {
    auto type = runtime.GetType(Types::kScene);

    count_ = new Property(type, "Count");

    get_item_ = new Method(type, "get_Item", 1);
    initialize_ = new Method(type, "Initialize", 0);
    terminate_ = new Method(type, "Terminate", 0);
    fixed_update_ = new Method(type, "FixedUpdate", 0);
    update_ = new Method(type, "Update", 0);
    render_ = new Method(type, "Render", 0);
    invalidate_ = new Method(type, "Invalidate", 0);
    create_game_object_ = new Method(type, "CreateGameObject", 0);
    delete_game_object_ = new Method(type, "DeleteGameObject", 1);
    serialize_ = new Method(type, "Serialize", 0);
    deserialize_ = new Method(type, "Deserialize", 1);
}

} // namespace engine