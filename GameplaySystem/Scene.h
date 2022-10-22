#pragma once
#include "GameObject.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_object.h"

class Scene
{
public:
   Scene():object_reference(nullptr){}
   explicit Scene(const mono::mono_assembly& assembly);
   
   GameObject* CreateGameObject() const;
   void DeleteGameObject(GameObject* game_object);

   void Initialize();
   void FixedUpdate();
   void Update();
   void Render();
   void Terminate();
   void Invalidate();
   
   const mono::mono_object& GetInternalPtr();

   static void CacheMethods(const mono::mono_assembly& assembly);

private:
   mono::mono_object* object_reference;
   
   static mono::mono_method_invoker* initialize_;
   static mono::mono_method_invoker* terminate_;
   static mono::mono_method_invoker* fixed_update_;
   static mono::mono_method_invoker* update_;
   static mono::mono_method_invoker* render_;
   static mono::mono_method_invoker* invalidate_;
   static mono::mono_method_invoker* create_game_object_;
   static mono::mono_method_invoker* delete_game_object_;
};
