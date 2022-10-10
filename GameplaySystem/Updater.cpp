#include "pch.h"
#include "Updater.h"

#include <cassert>

namespace engine {

//Updater::Updater(std::string method_name)
//	: method_name_(std::move(method_name))
//{}
//
//void Updater::Update() {
//	for (auto& object : subscribers_) {
//		object.CallMethod(method_name_);
//	}
//}
//
//void Updater::Subscribe(engine::CSharpObject object) {
//	MonoMethod* method = mono_class_get_method_from_name(object.GetMonoClass(), method_name_.c_str(), 0);
//	assert(method != nullptr);
//	subscribers_.insert(object);
//}
//
//bool Updater::TryToSubsribe(engine::CSharpObject object) {
//	MonoMethod* method = mono_class_get_method_from_name(object.GetMonoClass(), method_name_.c_str(), 0);
//
//	if (method != nullptr) {		
//		Subscribe(object);
//		return true;
//	} 
//
//	return false;
//}
//
//void Updater::Unsubscribe(engine::CSharpObject object) {
//	subscribers_.erase(object);
//}

};