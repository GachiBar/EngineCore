#include "GameObjectInspectorWindow.h"
#include "SimpleMath.h"
#include "imgui/imgui.h"
#include "../GameplaySystem/Component.h"
#include "../GameplaySystem/Engine.h"
#include "../monowrapper/monopp/mono_domain.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_string.h"

#include <mono/metadata/reflection.h>
#include <format>

std::shared_ptr<engine::Scene> GameObjectInspectorWindow::GetScene() const
{
	return scene;
}

void GameObjectInspectorWindow::SetScene(std::shared_ptr<engine::Scene> scene)
{
	this->scene = scene;
}

std::shared_ptr<engine::GameObject> GameObjectInspectorWindow::GetGameObject()
{
	return game_object;
}

void GameObjectInspectorWindow::SetGameObject(std::shared_ptr<engine::GameObject> gameObject)
{
	this->game_object = gameObject;

	if (game_object != nullptr) 
	{
		FindAvaliableComponents();
	}	
}

GameObjectInspectorWindow::GameObjectInspectorWindow(const mono::mono_assembly& assembly)
	: assembly(assembly)
	, game_objects_copasity(0)
{
	CacheComponentsData();
	available_components_items = new const char* [components_names.size()];
}

GameObjectInspectorWindow::~GameObjectInspectorWindow()
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

void GameObjectInspectorWindow::Draw()
{
	ImGui::Begin("Property Window");
	if (!game_object.get())
	{
		ImGui::End();
		return;
	}

	DrawGameObjectFields();

	for (size_t i = 0; i < game_object->Count(); ++i)
	{
		auto component = (*game_object)[i];
		DrawComponentFields(component);
	}

	ImGui::Separator();
	DrawAddComponentPanel();
	ImGui::End();	
}

void GameObjectInspectorWindow::CacheComponentsData()
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
			components_names.push_back(type.get_fullname());
		}
	}
}

void GameObjectInspectorWindow::DrawGameObjectFields()
{
	if (ImGui::CollapsingHeader("GameObject", ImGuiTreeNodeFlags_DefaultOpen)) 
	{
		auto nameField = game_object->GetField("_name");
		auto attributes = nameField.GetAttributes();
		DrawStringField(nameField, attributes);
	}
}

void GameObjectInspectorWindow::DrawComponentFields(std::shared_ptr<engine::Component> component)
{	
	bool visible = true;

	if (ImGui::CollapsingHeader(component->Name().c_str(), &visible, ImGuiTreeNodeFlags_DefaultOpen)) 
	{
		auto fields = component->GetFields();
		
		for (auto field : fields) 
		{
			auto attributes = field.GetAttributes();

			if (!IsEditableField(field, attributes)) 
			{
				continue;
			}	

			bool isFieldChanged = false;

			switch (field.GetTypeData().type)
			{
			case engine::Type::kFloat:
				isFieldChanged = DrawFloatField(field, attributes);
				break;
			case engine::Type::kDouble:
				isFieldChanged = DrawDoubleField(field, attributes);
				break;
			case engine::Type::kBool:
				isFieldChanged = DrawBoolField(field, attributes);
				break;
			case engine::Type::kByte:
				isFieldChanged = DrawByteField(field, attributes);
				break;
			case engine::Type::kShort:
				isFieldChanged = DrawShortField(field, attributes);
				break;
			case engine::Type::kInt:
				isFieldChanged = DrawIntField(field, attributes);
				break;
			case engine::Type::kLong:
				isFieldChanged = DrawLongField(field, attributes);
				break;
			case engine::Type::kUByte:
				isFieldChanged = DrawUByteField(field, attributes);
				break;
			case engine::Type::kUShort:
				isFieldChanged = DrawUShortField(field, attributes);
				break;
			case engine::Type::kUInt:
				isFieldChanged = DrawUIntField(field, attributes);
				break;
			case engine::Type::kULong:
				isFieldChanged = DrawULongField(field, attributes);
				break;
			case engine::Type::kVector2:
				isFieldChanged = DrawVector2Field(field, attributes);
				break;
			case engine::Type::kVector3:
				isFieldChanged = DrawVector3Field(field, attributes);
				break;
			case engine::Type::kVector4:
				isFieldChanged = DrawVector4Field(field, attributes);
				break;
			case engine::Type::kString:
				isFieldChanged = DrawStringField(field, attributes);
				break;
			case engine::Type::kGameObject:
				isFieldChanged = DrawGameObjectField(field, attributes);
				break;
			default:
				break;
			}

			if (isFieldChanged && component->HasMethod("Invalidate"))
			{
				component->GetMethod("Invalidate").Invoke();
			}
		}
	}

	if (!visible) 
	{
		game_object->RemoveComponent(component);
		game_object->Invalidate();
		FindAvaliableComponents();
	}
}

void GameObjectInspectorWindow::DrawAddComponentPanel()
{
	int selected = 0;

	if (ImGui::Combo("Add", &selected, available_components_items, avaliable_components_count))
	{		
		std::string fullName(available_components_items[selected]);
		std::string nameSpace;
		std::string name;
		ParseFullName(fullName, nameSpace, name);

		game_object->AddComponent(nameSpace, name);	
		game_object->Invalidate();
		FindAvaliableComponents();
	}
}

bool GameObjectInspectorWindow::DrawFloatField(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{	
	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue().value();
	auto value = monoObject.Unbox<float>();

	float min = 0;
	float max = 0;

	if (TryGetSliderConstraints(attributes, min, max))
	{
		if (ImGui::SliderFloat(fieldName.c_str(), &value, min, max))
		{
			field.SetValue(&value);
			return true;
		}
	}
	else 
	{
		if (ImGui::InputFloat(fieldName.c_str(), &value))
		{
			field.SetValue(&value);
			return true;
		}
	}

	return false;
}

bool GameObjectInspectorWindow::DrawDoubleField(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{	
	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue().value();
	auto value = monoObject.Unbox<double>();

	if (ImGui::InputDouble(fieldName.c_str(), &value))
	{
		field.SetValue(&value);
		return true;
	}

	return false;
}

bool GameObjectInspectorWindow::DrawBoolField(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{	
	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue().value();
	bool* value = reinterpret_cast<bool*>(monoObject.Unbox());

	if (ImGui::Checkbox(fieldName.c_str(), value))
	{
		field.SetValue(value);
		return true;
	}

	return false;
}

bool GameObjectInspectorWindow::DrawByteField(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{	
	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue().value();
	void* value = monoObject.Unbox();
	ImS8 step = 1;

	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_S8, value, &step))
	{
		field.SetValue(value);
		return true;
	}

	return false;
}

bool GameObjectInspectorWindow::DrawShortField(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{
	auto fieldName = GetFieldName(field, attributes);	
	auto monoObject = field.GetValue().value();
	void* value = monoObject.Unbox();
	ImS16 step = 1;

	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_S16, value, &step))
	{
		field.SetValue(value);
		return true;
	}

	return false;
}

bool GameObjectInspectorWindow::DrawIntField(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{	
	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue().value();
	void* value = monoObject.Unbox();
	ImS32 step = 1;

	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_S32, value, &step))
	{
		field.SetValue(value);
		return true;
	}

	return false;
}

bool GameObjectInspectorWindow::DrawLongField(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{	
	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue().value();
	void* value = monoObject.Unbox();
	ImS64 step = 1;
	
	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_S64, value, &step))
	{
		field.SetValue(value);
		return true;
	}

	return false;
}

bool GameObjectInspectorWindow::DrawUByteField(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{	
	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue().value();
	void* value = monoObject.Unbox();
	ImU8 step = 1;

	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_U8, value, &step))
	{
		field.SetValue(value);
		return true;
	}

	return false;
}

bool GameObjectInspectorWindow::DrawUShortField(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{	
	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue().value();
	void* value = monoObject.Unbox();
	ImU16 step = 1;

	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_U16, value, &step))
	{
		field.SetValue(value);
		return true;
	}

	return false;
}

bool GameObjectInspectorWindow::DrawUIntField(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{
	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue().value();
	void* value = monoObject.Unbox();
	ImU32 step = 1;

	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_U32, value, &step))
	{
		field.SetValue(value);
		return true;
	}

	return false;
}

bool GameObjectInspectorWindow::DrawULongField(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{
	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue().value();
	void* value = monoObject.Unbox();
	ImU64 step = 1;

	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_U64, value, &step))
	{
		field.SetValue(value);
		return true;
	}

	return false;
}

bool GameObjectInspectorWindow::DrawVector2Field(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{
	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue().value();
	auto value = monoObject.Unbox<DirectX::SimpleMath::Vector2>();
	float vector[2] = { value.x, value.y };

	float min = 0;
	float max = 0;

	if (TryGetSliderConstraints(attributes, min, max))
	{
		if (ImGui::SliderFloat2(fieldName.c_str(), vector, min, max))
		{
			field.SetValue(vector);
			return true;
		}
	}
	else
	{
		if (ImGui::InputFloat2(fieldName.c_str(), vector))
		{
			field.SetValue(vector);
			return true;
		}
	}

	return false;
}

bool GameObjectInspectorWindow::DrawVector3Field(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{
	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue().value();
	auto value = monoObject.Unbox<DirectX::SimpleMath::Vector3>();
	float vector[3] = { value.x, value.y, value.z };

	float min = 0;
	float max = 0;

	if (TryGetSliderConstraints(attributes, min, max))
	{
		if (ImGui::SliderFloat3(fieldName.c_str(), vector, min, max))
		{
			field.SetValue(vector);
			return true;
		}
	}
	else
	{
		if (ImGui::InputFloat3(fieldName.c_str(), vector))
		{
			field.SetValue(vector);
			return true;
		}
	}

	return false;
}

bool GameObjectInspectorWindow::DrawVector4Field(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{
	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue().value();
	auto value = monoObject.Unbox<DirectX::SimpleMath::Vector4>();
	float vector[4] = { value.x, value.y, value.z, value.w };

	float min = 0;
	float max = 0;

	if (TryGetSliderConstraints(attributes, min, max))
	{
		if (ImGui::SliderFloat4(fieldName.c_str(), vector, min, max))
		{
			field.SetValue(vector);
			return true;
		}
	}
	else
	{
		if (ImGui::InputFloat4(fieldName.c_str(), vector))
		{
			field.SetValue(vector);
			return true;
		}
	}

	return false;
}

bool GameObjectInspectorWindow::DrawStringField(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{
	const size_t bufferSize = 256;
	char buffer[bufferSize];
	buffer[0] = '\0';

	auto fieldName = GetFieldName(field, attributes);
	auto mono_object = field.GetValue();	

	if (mono_object.has_value()) 
	{
		mono::mono_string value(mono_object.value().GetInternal());
		std::string content = value.as_utf8();
		content.copy(buffer, bufferSize);
		buffer[content.size()] = '\0';
	}

	if (ImGui::InputText(fieldName.c_str(), buffer, bufferSize))
	{
		auto& domain = mono::mono_domain::get_current_domain();
		std::string newContent(buffer);
		mono::mono_string newValue(domain, newContent);
		field.SetValue(newValue);
		return true;
	}

	return false;
}

bool GameObjectInspectorWindow::DrawGameObjectField(
	engine::Field field,
	const std::vector<engine::Object>& attributes)
{
	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue();

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

	if (ImGui::Combo(fieldName.c_str(), &selected, game_objects_names, scene->Count()))
	{
		auto gameObject = game_objects_pointers[selected];
		field.SetValue(gameObject);
		return true;
	}

	return false;
}

void GameObjectInspectorWindow::ParseFullName(
	const std::string& fullName, 
	std::string& namespace_out, 
	std::string& name_out)
{
	size_t lastDotPosition = fullName.find_last_of(".");
	namespace_out = fullName.substr(0, lastDotPosition);
	name_out = fullName.substr(lastDotPosition + 1, fullName.size() - lastDotPosition - 1);
}

void GameObjectInspectorWindow::CopyAsNullTerminated(char* destination, const std::string& source)
{
	source.copy(destination, source.size());
	destination[source.size()] = '\0';
}

void GameObjectInspectorWindow::ChangeGameObjectResourcesCopasity(size_t size)
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

void GameObjectInspectorWindow::FindAvaliableComponents()
{
	std::unordered_set<std::string> addedComponents;

	for (size_t i = 0; i < game_object->Count(); ++i)
	{
		auto component = (*game_object)[i];
		auto componentType = component->GetInternal().get_type();
		addedComponents.insert(componentType.get_fullname());
	}

	avaliable_components_count = components_names.size() - addedComponents.size();
	auto temp = components_names.begin();

	for (size_t i = 0; i < avaliable_components_count; ++i)
	{
		while (addedComponents.contains(*temp))
		{
			std::advance(temp, 1);
		}

		available_components_items[i] = temp->data();
		std::advance(temp, 1);
	}
}

std::string GameObjectInspectorWindow::GetFieldName(
	const engine::Field& field,
	const std::vector<engine::Object>& attributes)
{
	auto predicate = [](auto attr)
	{
		return attr.GetTypeData().type == engine::Type::kInspectorNameAttribute;
	};

	auto nameAttribute = std::find_if(attributes.begin(), attributes.end(), predicate);

	if (nameAttribute == attributes.end())
	{
		return field.GetName();
	}

	auto nameProperty = nameAttribute->GetProperty("Name");
	auto value = nameProperty.GetValue().value();
	mono::mono_string result(value.GetInternal());
	return result.as_utf8();
}

bool GameObjectInspectorWindow::IsEditableField(
	const engine::Field& field, 
	const std::vector<engine::Object>& attributes)
{
	for (auto attr : attributes) 
	{
		if (attr.GetTypeData().type == engine::Type::kHideInInspectorAttribute) 
		{
			return false;
		}
	}

	for (auto attr : attributes) 
	{
		if (attr.GetTypeData().type == engine::Type::kSerializeFieldAttribute)
		{
			return true;
		}
	}

	return field.IsPublic();
}

bool GameObjectInspectorWindow::TryGetSliderConstraints(
	const std::vector<engine::Object>& attributes,
	float& min_out, 
	float& max_out)
{
	auto predicate = [](auto attr)
	{
		return attr.GetTypeData().type == engine::Type::kSliderAttribute;
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
