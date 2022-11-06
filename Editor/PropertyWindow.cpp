#include "PropertyWindow.h"
#include "SimpleMath.h"
#include "imgui/imgui.h"
#include "../GameplaySystem/Component.h"
#include "../monowrapper/monopp/mono_domain.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_string.h"

#include <mono/metadata/reflection.h>
#include <format>

const std::string PropertyWindow::kHideInInspectorAttribute = "GameplayCore.Serialization.HideInInspectorAttribute";
const std::string PropertyWindow::kSerializeFieldAttribute = "GameplayCore.Serialization.SerializeFieldAttribute";
const std::string PropertyWindow::kInspectorNameAttribute = "GameplayCore.Editor.InspectorNameAttribute";

bool operator==(const ComponentData& lhs, const ComponentData& rhs)
{
	return lhs.NameSpace == rhs.NameSpace && lhs.Name == rhs.Name;
}

std::shared_ptr<engine::Scene> PropertyWindow::GetScene()
{
	return scene;
}

void PropertyWindow::SetScene(std::shared_ptr<engine::Scene> scene) 
{
	this->scene = scene;
	this->game_object = nullptr;
	// TODO: bind to GameObjectAdded
	// TODO: bind to GameObjectRemoved.
}

std::shared_ptr<engine::GameObject> PropertyWindow::GetGameObject()
{
	return game_object;
}

void PropertyWindow::SetGameObject(std::shared_ptr<engine::GameObject> gameObject)
{
	this->game_object = gameObject;
	// TODO:	Update game object list.
	// TODO: bind to ComponentAdded.
	// TODO: bind to ComponentRemoved.
	// TODO:	Update avaliable components list.
}

PropertyWindow::PropertyWindow(const mono::mono_assembly& assembly)
	: assembly(assembly)
	, game_objects_copasity(0)
{
	CacheComponentsData();
	available_components_items = new const char* [components_names.size()];
}

PropertyWindow::~PropertyWindow()
{
	delete[] available_components_items;

	if (game_objects_copasity > 0)
	{
		for (size_t i = 0; i < game_objects_copasity; ++i)
		{
			delete[] game_objects_names[i];
		}

		delete[] game_objects_pointers;
		delete[] game_objects_names;
	}
}

void PropertyWindow::draw_imgui()
{
	ImGui::Begin("Property Window");
	if (!game_object.get())
	{
		ImGui::End();
		return;
	}

	DrawGameObjectProperties();
	added_components.clear();

	for (size_t i = 0; i < game_object->Count(); ++i)
	{
		// We nead push id to allow multiple buttons with same names ("Remove").
		ImGui::PushID(i);

		auto component = (*game_object)[i];
		auto componentType = component->GetInternal().get_type();
		added_components.insert({ componentType.get_namespace(), componentType.get_name() });

		DrawComponentProperties(component);
		ImGui::PopID();
	}

	DrawAddComponentPanel();
	ImGui::End();
}

void PropertyWindow::CacheComponentsData()
{
	auto baseComponentType = assembly.get_type("GameplayCore.Components", "Component");
	auto typeNames = assembly.dump_type_names();

	for (auto typeName : typeNames)
	{
		mono::mono_type type;

		try
		{
			std::string nameSpace;
			std::string name;
			ParseFullName(typeName, nameSpace, name);
			type = assembly.get_type(nameSpace, name);
		}
		catch (mono::mono_exception& ex)
		{
			continue;
		}

		bool isComponent = false;

		while (type.has_base_type() && !type.is_abstract())
		{
			if (type.is_derived_from(baseComponentType))
			{
				isComponent = true;
				break;
			}
			
			type = type.get_base_type();
		}

		if (isComponent)
		{
			components_names.push_back({ type.get_namespace(), type.get_name() });
		}
	}
}

void PropertyWindow::DrawGameObjectProperties()
{
	if (ImGui::CollapsingHeader("GameObject", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text(std::format("{}: {}", "Name", game_object->Name()).c_str());
	}
}

void PropertyWindow::DrawComponentProperties(std::shared_ptr<engine::Component> component)
{	
	if (ImGui::CollapsingHeader(component->Name().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		auto properties = component->GetProperties();
		
		for (auto property : properties) 
		{
			auto attributes = property.GetAttributes();

			if (!IsEditableProperty(property, attributes)) 
			{
				continue;
			}	

			switch (property.GetType())
			{
			case engine::PropertyType::kFloat:
				DrawFloatProperty(property, attributes);
				break;
			case engine::PropertyType::kDouble:
				DrawDoubleProperty(property, attributes);
				break;
			case engine::PropertyType::kBool:
				DrawBoolProperty(property, attributes);
				break;
			case engine::PropertyType::kByte:
				DrawByteProperty(property, attributes);
				break;
			case engine::PropertyType::kShort:
				DrawShortProperty(property, attributes);
				break;
			case engine::PropertyType::kInt:
				DrawIntProperty(property, attributes);
				break;
			case engine::PropertyType::kLong:
				DrawLongProperty(property, attributes);
				break;
			case engine::PropertyType::kUByte:
				DrawUByteProperty(property, attributes);
				break;
			case engine::PropertyType::kUShort:
				DrawUShortProperty(property, attributes);
				break;
			case engine::PropertyType::kUInt:
				DrawUIntProperty(property, attributes);
				break;
			case engine::PropertyType::kULong:
				DrawULongProperty(property, attributes);
				break;
			case engine::PropertyType::kVector2:
				DrawVector2Property(property, attributes);
				break;
			case engine::PropertyType::kVector3:
				DrawVector3Property(property, attributes);
				break;
			case engine::PropertyType::kVector4:
				DrawVector4Property(property, attributes);
				break;
			case engine::PropertyType::kString:
				DrawStringProperty(property, attributes);
				break;
			case engine::PropertyType::kGameObject:
				DrawGameObjectProperty(property, attributes);
				break;
			case engine::PropertyType::kUndefined:
			default:
				break;
			}
		}

		if (ImGui::Button("Remove")) 
		{
			game_object->RemoveComponent(component);
		}
	}
}

void PropertyWindow::DrawAddComponentPanel()
{
	size_t availableComponentsCount = components_names.size() - added_components.size();
	auto temp = components_names.begin();

	for (size_t i = 0; i < availableComponentsCount; ++i) 
	{
		while (added_components.contains(*temp)) 
		{
			std::advance(temp, 1);
		}

		available_components_items[i] = temp->FullName.data();
		std::advance(temp, 1);
	}

	int selected = 0;

	if (ImGui::Combo("Add", &selected, available_components_items, availableComponentsCount))
	{		
		std::string fullName(available_components_items[selected]);
		std::string nameSpace;
		std::string name;
		ParseFullName(fullName, nameSpace, name);

		game_object->AddComponent(nameSpace, name);
		game_object->Invalidate();
	}
}

void PropertyWindow::DrawFloatProperty(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	auto value = reinterpret_cast<float*>(monoObject.unbox());

	if (ImGui::InputFloat(propertyName.c_str(), value))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawDoubleProperty(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	auto value = reinterpret_cast<double*>(monoObject.unbox());

	if (ImGui::InputDouble(propertyName.c_str(), value))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawBoolProperty(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	bool* value = reinterpret_cast<bool*>(monoObject.unbox());

	if (ImGui::Checkbox(propertyName.c_str(), value))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawByteProperty(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImS8 step = 1;

	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_S8, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawShortProperty(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{
	auto propertyName = GetPropertyName(property, attributes);	
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImS16 step = 1;

	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_S16, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawIntProperty(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImS32 step = 1;

	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_S32, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawLongProperty(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImS64 step = 1;
	
	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_S64, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawUByteProperty(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImU8 step = 1;

	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_U8, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawUShortProperty(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImU16 step = 1;

	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_U16, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawUIntProperty(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImU32 step = 1;

	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_U32, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawULongProperty(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImU64 step = 1;

	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_U64, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawVector2Property(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	auto value = reinterpret_cast<DirectX::SimpleMath::Vector2*>(monoObject.unbox());
	float vector[2] = { value->x, value->y };

	if (ImGui::InputFloat2(propertyName.c_str(), vector))
	{
		property.SetValue(vector);
	}
}

void PropertyWindow::DrawVector3Property(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	auto value = reinterpret_cast<DirectX::SimpleMath::Vector3*>(monoObject.unbox());
	float vector[3] = { value->x, value->y, value->z };

	if (ImGui::InputFloat3(propertyName.c_str(), vector))
	{
		property.SetValue(vector);
	}
}

void PropertyWindow::DrawVector4Property(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	auto value = reinterpret_cast<DirectX::SimpleMath::Vector4*>(monoObject.unbox());
	float vector[4] = { value->x, value->y, value->z, value->w };

	if (ImGui::InputFloat4(propertyName.c_str(), vector))
	{
		property.SetValue(vector);
	}
}

void PropertyWindow::DrawStringProperty(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{
	const size_t bufferSize = 256;
	char buffer[bufferSize];
	buffer[0] = '\0';

	auto propertyName = GetPropertyName(property, attributes);
	auto mono_object = property.GetValue();	

	if (mono_object.has_value()) 
	{
		mono::mono_string value(mono_object.value());
		std::string content = value.as_utf8();
		content.copy(buffer, bufferSize);
		buffer[content.size()] = '\0';
	}

	if (ImGui::InputText(propertyName.c_str(), buffer, bufferSize))
	{
		auto& domain = mono::mono_domain::get_current_domain();
		std::string newContent(buffer);
		mono::mono_string newValue(domain, newContent);
		property.SetValue(newValue);
	}
}

void PropertyWindow::DrawGameObjectProperty(
	engine::ComponentProperty property,
	const std::vector<mono::mono_object>& attributes)
{
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue();

	if (scene->Count() > game_objects_copasity)
	{
		ChangeGameObjectResourcesCopasity(scene->Count() * 2);
	}

	auto current = game_objects_names;
	auto tempPointers = game_objects_pointers;
	auto tempNames = game_objects_names;

	tempPointers[0] = nullptr;
	CopyAsNullTerminated(*tempNames, "None");

	std::advance(tempPointers, 1);
	std::advance(tempNames, 1);

	for (size_t i = 0; i < scene->Count(); ++i) 
	{
		auto otherGameObject = (*scene)[i];

		if (*otherGameObject == *game_object) 
		{
			continue;
		}
		if (monoObject.has_value() && *otherGameObject == monoObject.value())
		{
			current = tempNames;
		}

		tempPointers[0] = otherGameObject->GetInternal().get_internal_ptr();
		CopyAsNullTerminated(*tempNames, otherGameObject->Name());

		std::advance(tempPointers, 1);
		std::advance(tempNames, 1);
	}

	int selected = std::distance(game_objects_names, current);

	if (ImGui::Combo(propertyName.c_str(), &selected, game_objects_names, scene->Count()))
	{
		auto gameObject = game_objects_pointers[selected];
		property.SetValue(gameObject);
	}
}

void PropertyWindow::ParseFullName(
	const std::string& fullName, 
	std::string& namespace_out, 
	std::string& name_out)
{
	size_t lastDotPosition = fullName.find_last_of(".");
	namespace_out = fullName.substr(0, lastDotPosition);
	name_out = fullName.substr(lastDotPosition + 1, fullName.size() - lastDotPosition - 1);
}

void PropertyWindow::CopyAsNullTerminated(char* destination, const std::string& source)
{
	source.copy(destination, source.size());
	destination[source.size()] = '\0';
}

void PropertyWindow::ChangeGameObjectResourcesCopasity(size_t size)
{
	if (game_objects_copasity > 0)
	{
		for (size_t i = 0; i < game_objects_copasity; ++i)
		{
			delete[] game_objects_names[i];
		}

		delete[] game_objects_pointers;
		delete[] game_objects_names;
	}

	game_objects_copasity = size;
	game_objects_pointers = new void* [game_objects_copasity];
	game_objects_names = new char* [game_objects_copasity];

	for (size_t i = 0; i < game_objects_copasity; ++i)
	{
		game_objects_names[i] = new char[kGameObjectNameMaxSize];
	}
}

bool PropertyWindow::IsEditableProperty(
	const engine::ComponentProperty& property, 
	const std::vector<mono::mono_object>& attributes)
{
	try 
	{
		auto internal = property.GetInternal();
		auto getter = internal.get_get_method();
		auto setter = internal.get_set_method();

		if (getter.get_visibility() == mono::visibility::vis_public && 
			setter.get_visibility() == mono::visibility::vis_public)
		{
			auto predicate = [](auto attr)
			{
				return IsFullNameEqualTo(attr, kHideInInspectorAttribute);
			};

			return std::find_if(attributes.begin(), attributes.end(), predicate) == attributes.end();
		}
		else
		{
			auto predicate = [](auto attr) 
			{
				return IsFullNameEqualTo(attr, kSerializeFieldAttribute);
			};

			return std::find_if(attributes.begin(), attributes.end(), predicate) != attributes.end();
		}
	}
	catch(const mono::mono_exception&)	
	{
		return false;
	}
}

bool PropertyWindow::IsFullNameEqualTo(
	const mono::mono_object& obj,
	const std::string& fullname)
{
	return obj.get_type().get_fullname() == fullname;
}

std::string PropertyWindow::GetPropertyName(
	const engine::ComponentProperty& property,
	const std::vector<mono::mono_object>& attributes) 
{
	auto predicate = [](auto attr)
	{
		return attr.get_type().get_fullname() == kInspectorNameAttribute;
	};

	auto nameAttribute = std::find_if(attributes.begin(), attributes.end(), predicate);

	if (nameAttribute == attributes.end()) 
	{
		return property.GetName();
	}
	
	auto name_property = nameAttribute->get_type().get_property("Name");
	mono::mono_property_invoker invoker(name_property);
	mono::mono_object value(invoker.get_value(*nameAttribute));
	mono::mono_string result(value);
	return result.as_utf8();
}
