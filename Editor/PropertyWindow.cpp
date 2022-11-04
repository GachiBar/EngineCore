#include "PropertyWindow.h"
#include "SimpleMath.h"
#include "imgui/imgui.h"
#include "../GameplaySystem/Component.h"
#include "../monowrapper/monopp/mono_domain.h"
#include "../monowrapper/monopp/mono_method.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_property_invoker.h"
#include "../monowrapper/monopp/mono_string.h"
#include "../Editor/InputSystem/InputManager.h"

#include <format>

bool operator==(const ComponentData& lhs, const ComponentData& rhs)
{
	return lhs.NameSpace == rhs.NameSpace && lhs.Name == rhs.Name;
}

PropertyWindow::PropertyWindow(const mono::mono_assembly& assembly)
	: assembly(assembly)
	, scene_game_objects_names_size(0)
{
	CacheComponentsData();
	available_components_items = new const char* [components_names.size()];
}

PropertyWindow::~PropertyWindow()
{
	delete[] available_components_items;

	if (scene_game_objects_names_size > 0)
	{
		for (size_t i = 0; i < scene_game_objects_names_size; ++i)
		{
			delete[] scene_game_objects_names[i];
		}

		delete[] scene_game_objects_names;
	}
}

void PropertyWindow::draw_imgui(
	std::shared_ptr<engine::Scene> scene, 
	std::shared_ptr<engine::GameObject> gameObject)
{
	ImGui::Begin("Property Window");
	if (!gameObject.get())
	{
		ImGui::End();
		return;
	}

	DrawGameObjectProperties(gameObject);
	added_components.clear();

	for (size_t i = 0; i < gameObject->Count(); ++i)
	{
		// We nead push id to allow multiple buttons with same names ("Remove").
		ImGui::PushID(i);

		auto component = (*gameObject)[i];
		auto componentType = component->GetInternal().get_type();
		added_components.insert({ componentType.get_namespace(), componentType.get_name() });

		DrawComponentProperties(scene, gameObject, component);
		ImGui::PopID();
	}

	DrawAddComponentPanel(gameObject);
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

void PropertyWindow::DrawGameObjectProperties(std::shared_ptr<engine::GameObject> gameObject)
{
	if (ImGui::CollapsingHeader("GameObject", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text(std::format("{}: {}", "Name", gameObject->Name()).c_str());
	}
}

void PropertyWindow::DrawComponentProperties(
	std::shared_ptr<engine::Scene> scene,
	std::shared_ptr<engine::GameObject> gameObject, 
	std::shared_ptr<engine::Component> component)
{	
	if (ImGui::CollapsingHeader(component->Name().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		auto editablePropertiesNames = component->GetEditablePropertiesNames();

		for (auto propertyName : editablePropertiesNames)
		{
			auto property = component->GetProperty(propertyName);

			switch (property.GetType())
			{
			case engine::PropertyType::kFloat:
				DrawFloatProperty(property);
				break;
			case engine::PropertyType::kDouble:
				DrawDoubleProperty(property);
				break;
			case engine::PropertyType::kBool:
				DrawBoolProperty(property);
				break;	
			case engine::PropertyType::kByte:
				DrawByteProperty(property);
				break;
			case engine::PropertyType::kShort:
				DrawShortProperty(property);
				break;
			case engine::PropertyType::kInt:
				DrawIntProperty(property);
				break;
			case engine::PropertyType::kLong:
				DrawLongProperty(property);
				break;
			case engine::PropertyType::kUByte:
				DrawUByteProperty(property);
				break;
			case engine::PropertyType::kUShort:
				DrawUShortProperty(property);
				break;
			case engine::PropertyType::kUInt:
				DrawUIntProperty(property);
				break;
			case engine::PropertyType::kULong:
				DrawULongProperty(property);
				break;
			case engine::PropertyType::kVector2:
				DrawVector2Property(property);
				break;
			case engine::PropertyType::kVector3:
				DrawVector3Property(property);
				break;
			case engine::PropertyType::kVector4:
				DrawVector4Property(property);
				break;
			case engine::PropertyType::kString:
				DrawStringProperty(property);
				break;
			case engine::PropertyType::kGameObject:
				DrawGameObjectProperty(scene, gameObject, property);
				break;
			case engine::PropertyType::kUndefined:
			default:
				break;
			}
		}

		if (ImGui::Button("Remove")) {
			gameObject->RemoveComponent(component);
		}
	}
}

void PropertyWindow::DrawAddComponentPanel(std::shared_ptr<engine::GameObject> gameObject)
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

		gameObject->AddComponent(nameSpace, name);
		gameObject->Invalidate();
	}
}

void PropertyWindow::DrawFloatProperty(engine::ComponentProperty property)
{
	// We can just get value, because value types can not be null.
	auto monoObject = property.GetValue().value();
	auto value = reinterpret_cast<float*>(monoObject.unbox());

	if (ImGui::InputFloat(property.GetName().c_str(), value))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawDoubleProperty(engine::ComponentProperty property)
{
	// We can just get value, because value types can not be null.
	auto monoObject = property.GetValue().value();
	auto value = reinterpret_cast<double*>(monoObject.unbox());

	if (ImGui::InputDouble(property.GetName().c_str(), value))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawBoolProperty(engine::ComponentProperty property)
{
	// We can just get value, because value types can not be null.
	auto monoObject = property.GetValue().value();
	bool* value = reinterpret_cast<bool*>(monoObject.unbox());

	if (ImGui::Checkbox(property.GetName().c_str(), value))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawByteProperty(engine::ComponentProperty property)
{
	// We can just get value, because value types can not be null.
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImS8 step = 1;

	if (ImGui::InputScalar(property.GetName().c_str(), ImGuiDataType_S8, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawShortProperty(engine::ComponentProperty property)
{
	// We can just get value, because value types can not be null.
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImS16 step = 1;

	if (ImGui::InputScalar(property.GetName().c_str(), ImGuiDataType_S16, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawIntProperty(engine::ComponentProperty property)
{
	// We can just get value, because value types can not be null.
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImS32 step = 1;

	if (ImGui::InputScalar(property.GetName().c_str(), ImGuiDataType_S32, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawLongProperty(engine::ComponentProperty property)
{
	// We can just get value, because value types can not be null.
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImS64 step = 1;
	
	if (ImGui::InputScalar(property.GetName().c_str(), ImGuiDataType_S64, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawUByteProperty(engine::ComponentProperty property)
{
	// We can just get value, because value types can not be null.
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImU8 step = 1;

	if (ImGui::InputScalar(property.GetName().c_str(), ImGuiDataType_U8, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawUShortProperty(engine::ComponentProperty property)
{
	// We can just get value, because value types can not be null.
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImU16 step = 1;

	if (ImGui::InputScalar(property.GetName().c_str(), ImGuiDataType_U16, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawUIntProperty(engine::ComponentProperty property)
{
	// We can just get value, because value types can not be null.
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImU32 step = 1;

	if (ImGui::InputScalar(property.GetName().c_str(), ImGuiDataType_U32, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawULongProperty(engine::ComponentProperty property)
{
	// We can just get value, because value types can not be null.
	auto monoObject = property.GetValue().value();
	void* value = monoObject.unbox();
	ImU64 step = 1;

	if (ImGui::InputScalar(property.GetName().c_str(), ImGuiDataType_U64, value, &step))
	{
		property.SetValue(value);
	}
}

void PropertyWindow::DrawVector2Property(engine::ComponentProperty property)
{
	// We can just get value, because value types can not be null.
	auto monoObject = property.GetValue().value();
	auto value = reinterpret_cast<DirectX::SimpleMath::Vector2*>(monoObject.unbox());
	float vector[2] = { value->x, value->y };

	if (ImGui::InputFloat2(property.GetName().c_str(), vector))
	{
		property.SetValue(vector);
	}
}

void PropertyWindow::DrawVector3Property(engine::ComponentProperty property)
{
	// We can just get value, because value types can not be null.
	auto monoObject = property.GetValue().value();
	auto value = reinterpret_cast<DirectX::SimpleMath::Vector3*>(monoObject.unbox());
	float vector[3] = { value->x, value->y, value->z };

	if (ImGui::InputFloat3(property.GetName().c_str(), vector))
	{
		property.SetValue(vector);
	}
}

void PropertyWindow::DrawVector4Property(engine::ComponentProperty property)
{
	// We can just get value, because value types can not be null.
	auto monoObject = property.GetValue().value();
	auto value = reinterpret_cast<DirectX::SimpleMath::Vector4*>(monoObject.unbox());
	float vector[4] = { value->x, value->y, value->z, value->w };

	if (ImGui::InputFloat4(property.GetName().c_str(), vector))
	{
		property.SetValue(vector);
	}
}

void PropertyWindow::DrawStringProperty(engine::ComponentProperty property)
{
	const size_t bufferSize = 256;
	char buffer[bufferSize];
	buffer[0] = '\0';

	auto mono_object = property.GetValue();	

	if (mono_object.has_value()) 
	{
		mono::mono_string value(mono_object.value());
		std::string content = value.as_utf8();
		content.copy(buffer, bufferSize);
		buffer[content.size()] = '\0';
	}

	if (ImGui::InputText(property.GetName().c_str(), buffer, bufferSize)) 
	{
		auto domain = mono::mono_domain::get_current_domain();
		std::string newContent(buffer);		
		mono::mono_string newValue(domain, newContent);
		property.SetValue(newValue);
	}
}

void PropertyWindow::DrawGameObjectProperty(
	std::shared_ptr<engine::Scene> scene,
	std::shared_ptr<engine::GameObject> gameObject, 
	engine::ComponentProperty property)
{
	static const std::string none = "None";
	auto monoObject = property.GetValue();

	if (scene->Count() > scene_game_objects_names_size) 
	{
		ChangeSceneGameObjectNamesSize(scene->Count() * 2);
	}

	auto current = scene_game_objects_names;
	auto temp = scene_game_objects_names;
	CopyAsNullTerminated(*temp, none);
	std::advance(temp, 1);

	for (size_t i = 0; i < scene->Count(); ++i) 
	{
		auto otherGameObject = (*scene)[i];

		if (otherGameObject == gameObject) 
		{
			continue;
		}
		if (monoObject.has_value() && *otherGameObject == monoObject.value())
		{
			current = temp;
		}

		auto name = otherGameObject->Name();
		CopyAsNullTerminated(*temp, name);
		std::advance(temp, 1);
	}

	int selected = std::distance(scene_game_objects_names, current);

	if (ImGui::Combo("Add", &selected, scene_game_objects_names, scene->Count()))
	{
		std::string name(scene_game_objects_names[selected]);
		// TODO: use name to find game object in scene and set as property value.
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

void PropertyWindow::ChangeSceneGameObjectNamesSize(size_t size)
{
	if (scene_game_objects_names_size > 0) 
	{
		for (size_t i = 0; i < scene_game_objects_names_size; ++i)
		{
			delete[] scene_game_objects_names[i];
		}

		delete[] scene_game_objects_names;
	}

	scene_game_objects_names_size = size;
	scene_game_objects_names = new char* [scene_game_objects_names_size];

	for (size_t i = 0; i < scene_game_objects_names_size; ++i)
	{
		scene_game_objects_names[i] = new char[kGameObjectNameMaxSize];
	}
}
