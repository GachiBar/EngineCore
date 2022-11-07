#include "PropertyWindow.h"
#include "SimpleMath.h"
#include "imgui/imgui.h"
#include "../GameplaySystem/Component.h"
#include "../monowrapper/monopp/mono_domain.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_string.h"

#include <mono/metadata/reflection.h>
#include <format>

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
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	auto value = monoObject.Unbox<float>();

	float min = 0;
	float max = 0;

	if (IsSliderProperty(attributes, min, max)) 
	{
		if (ImGui::SliderFloat(propertyName.c_str(), &value, min, max))
		{
			property.SetValue(&value);
		}
	}
	else 
	{
		if (ImGui::InputFloat(propertyName.c_str(), &value))
		{
			property.SetValue(&value);
		}
	}
}

void PropertyWindow::DrawDoubleProperty(
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	auto value = monoObject.Unbox<double>();

	if (ImGui::InputDouble(propertyName.c_str(), &value))
	{
		property.SetValue(&value);
	}
}

void PropertyWindow::DrawBoolProperty(
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	bool* value = reinterpret_cast<bool*>(monoObject.Unbox());

	if (ImGui::Checkbox(propertyName.c_str(), value))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawByteProperty(
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	void* value = monoObject.Unbox();
	ImS8 step = 1;

	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_S8, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawShortProperty(
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{
	auto propertyName = GetPropertyName(property, attributes);	
	auto monoObject = property.GetValue().value();
	void* value = monoObject.Unbox();
	ImS16 step = 1;

	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_S16, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawIntProperty(
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	void* value = monoObject.Unbox();
	ImS32 step = 1;

	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_S32, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawLongProperty(
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	void* value = monoObject.Unbox();
	ImS64 step = 1;
	
	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_S64, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawUByteProperty(
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	void* value = monoObject.Unbox();
	ImU8 step = 1;

	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_U8, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawUShortProperty(
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{	
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	void* value = monoObject.Unbox();
	ImU16 step = 1;

	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_U16, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawUIntProperty(
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	void* value = monoObject.Unbox();
	ImU32 step = 1;

	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_U32, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawULongProperty(
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	void* value = monoObject.Unbox();
	ImU64 step = 1;

	if (ImGui::InputScalar(propertyName.c_str(), ImGuiDataType_U64, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawVector2Property(
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	auto value = monoObject.Unbox<DirectX::SimpleMath::Vector2>();
	float vector[2] = { value.x, value.y };

	float min = 0;
	float max = 0;

	if (IsSliderProperty(attributes, min, max))
	{
		if (ImGui::SliderFloat2(propertyName.c_str(), vector, min, max))
		{
			property.SetValue(vector);
		}
	}
	else
	{
		if (ImGui::InputFloat2(propertyName.c_str(), vector))
		{
			property.SetValue(vector);
		}
	}
}

void PropertyWindow::DrawVector3Property(
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	auto value = monoObject.Unbox<DirectX::SimpleMath::Vector3>();
	float vector[3] = { value.x, value.y, value.z };

	float min = 0;
	float max = 0;

	if (IsSliderProperty(attributes, min, max))
	{
		if (ImGui::SliderFloat3(propertyName.c_str(), vector, min, max))
		{
			property.SetValue(vector);
		}
	}
	else
	{
		if (ImGui::InputFloat3(propertyName.c_str(), vector))
		{
			property.SetValue(vector);
		}
	}
}

void PropertyWindow::DrawVector4Property(
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue().value();
	auto value = monoObject.Unbox<DirectX::SimpleMath::Vector4>();
	float vector[4] = { value.x, value.y, value.z, value.w };

	float min = 0;
	float max = 0;

	if (IsSliderProperty(attributes, min, max))
	{
		if (ImGui::SliderFloat4(propertyName.c_str(), vector, min, max))
		{
			property.SetValue(vector);
		}
	}
	else
	{
		if (ImGui::InputFloat4(propertyName.c_str(), vector))
		{
			property.SetValue(vector);
		}
	}
}

void PropertyWindow::DrawStringProperty(
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{
	const size_t bufferSize = 256;
	char buffer[bufferSize];
	buffer[0] = '\0';

	auto propertyName = GetPropertyName(property, attributes);
	auto mono_object = property.GetValue();	

	if (mono_object.has_value()) 
	{
		mono::mono_string value(mono_object.value().GetInternal());
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
	engine::Property property,
	const std::vector<engine::Attribute>& attributes)
{
	auto propertyName = GetPropertyName(property, attributes);
	auto monoObject = property.GetValue();

	if (scene->Count() > game_objects_copasity)
	{
		auto degree = std::ceil(std::log2(scene->Count()));
		ChangeGameObjectResourcesCopasity(std::pow(2, degree));
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

std::string PropertyWindow::GetPropertyName(
	const engine::Property& property,
	const std::vector<engine::Attribute>& attributes)
{
	auto predicate = [](auto attr)
	{
		return attr.GetType() == engine::AttributeType::kInspectorNameAttribute;
	};

	auto nameAttribute = std::find_if(attributes.begin(), attributes.end(), predicate);

	if (nameAttribute == attributes.end())
	{
		return property.GetName();
	}

	auto nameProperty = nameAttribute->GetProperty("Name");
	auto value = nameProperty.GetValue().value();
	mono::mono_string result(value.GetInternal());
	return result.as_utf8();
}

bool PropertyWindow::IsEditableProperty(
	const engine::Property& property, 
	const std::vector<engine::Attribute>& attributes)
{
	if (property.CanRead() && property.CanWrite()) 
	{
		if (property.GetGetMethod().IsPublic() && 
			property.GetSetMethod().IsPublic()) 
		{
			auto predicate = [](auto attr)
			{
				return attr.GetType() == engine::AttributeType::kHideInInspectorAttribute;
			};

			return std::find_if(attributes.begin(), attributes.end(), predicate) == attributes.end();
		}
		else 
		{
			auto predicate = [](auto attr)
			{
				return attr.GetType() == engine::AttributeType::kSerializeFieldAttribute;
			};

			return std::find_if(attributes.begin(), attributes.end(), predicate) != attributes.end();
		}
	}

	return false;
}

bool PropertyWindow::IsSliderProperty(
	const std::vector<engine::Attribute>& attributes,
	float& min_out, 
	float& max_out)
{
	auto predicate = [](auto attr)
	{
		return attr.GetType() == engine::AttributeType::kSliderAttribute;
	};

	auto it = std::find_if(attributes.begin(), attributes.end(), predicate);

	if (it == attributes.end()) 
	{
		return false;
	}

	auto minProperty = it->GetProperty("Min");
	auto minValue = minProperty.GetValue().value();
	min_out = minValue.Unbox<float>();

	auto maxProperty = it->GetProperty("Max");
	auto maxValue = maxProperty.GetValue().value();
	max_out = maxValue.Unbox<float>();

	return true;
}
