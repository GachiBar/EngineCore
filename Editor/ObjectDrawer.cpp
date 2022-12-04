#include "ObjectDrawer.h"

#include <imgui/imgui.h>
#include <SimpleMath.h>

std::shared_ptr<engine::Scene> ObjectDrawer::GetScene() const
{
    return scene;
}

void ObjectDrawer::SetScene(std::shared_ptr<engine::Scene> scene)
{
    this->scene = scene;
}

ObjectDrawer::ObjectDrawer()
    : game_objects_pointers(nullptr)
    , game_objects_names(nullptr)
    , game_objects_copasity(0)
{}

ObjectDrawer::~ObjectDrawer()
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
}

bool ObjectDrawer::DrawObject(engine::Object& object)
{
	auto fields = object.GetFields();
	bool isFieldChanged = false;

	for (auto field : fields) 
	{
		if (field.GetTypeData() == engine::Types::kSingle) 
		{
			isFieldChanged |= DrawFloatField(field);
		} 
		else if (field.GetTypeData() == engine::Types::kDouble) 
		{
			isFieldChanged |= DrawDoubleField(field);
		} 
		else if (field.GetTypeData() == engine::Types::kBoolean) 
		{
			isFieldChanged |= DrawBoolField(field);
		}
		else if (field.GetTypeData() == engine::Types::kSByte) 
		{
			isFieldChanged |= DrawByteField(field);
		}
		else if (field.GetTypeData() == engine::Types::kInt16) 
		{
			isFieldChanged |= DrawShortField(field);
		}
		else if (field.GetTypeData() == engine::Types::kInt32)
		{
			isFieldChanged |= DrawIntField(field);
		}
		else if (field.GetTypeData() == engine::Types::kInt64)
		{
			isFieldChanged |= DrawLongField(field);
		}
		else if (field.GetTypeData() == engine::Types::kByte)
		{
			isFieldChanged |= DrawUByteField(field);
		}
		else if (field.GetTypeData() == engine::Types::kUInt16) 
		{
			isFieldChanged |= DrawUShortField(field);
		}
		else if (field.GetTypeData() == engine::Types::kUInt32)
		{
			isFieldChanged |= DrawUIntField(field);
		}
		else if (field.GetTypeData() == engine::Types::kUInt64)
		{
			isFieldChanged |= DrawULongField(field);
		}
		else if (field.GetTypeData() == engine::Types::kVector2)
		{
			isFieldChanged |= DrawVector2Field(field);
		}
		else if (field.GetTypeData() == engine::Types::kVector3)
		{
			isFieldChanged |= DrawVector3Field(field);
		}
		else if (field.GetTypeData() == engine::Types::kVector4)
		{
			isFieldChanged |= DrawVector4Field(field);
		}
		else if (field.GetTypeData() == engine::Types::kString)
		{
			isFieldChanged |= DrawStringField(field);
		}
		else if (field.GetTypeData() == engine::Types::kGameObject) 
		{
			isFieldChanged |= DrawGameObjectField(field);
		}
	}

	return isFieldChanged;
}

bool ObjectDrawer::DrawFloatField(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

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

bool ObjectDrawer::DrawDoubleField(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

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

bool ObjectDrawer::DrawBoolField(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

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

bool ObjectDrawer::DrawByteField(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

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

bool ObjectDrawer::DrawShortField(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

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

bool ObjectDrawer::DrawIntField(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

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

bool ObjectDrawer::DrawLongField(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

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

bool ObjectDrawer::DrawUByteField(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

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

bool ObjectDrawer::DrawUShortField(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

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

bool ObjectDrawer::DrawUIntField(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

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

bool ObjectDrawer::DrawULongField(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

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

bool ObjectDrawer::DrawVector2Field(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

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

bool ObjectDrawer::DrawVector3Field(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

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

bool ObjectDrawer::DrawVector4Field(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

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

bool ObjectDrawer::DrawStringField(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

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

bool ObjectDrawer::DrawGameObjectField(engine::Field field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue();
	auto c = scene->Count();

	if (scene->Count() + 1 > game_objects_copasity)
	{
		auto degree = std::ceil(std::log2(scene->Count() + 1));
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

	if (ImGui::Combo(fieldName.c_str(), &selected, game_objects_names, scene->Count() + 1))
	{
		auto gameObject = game_objects_pointers[selected];
		field.SetValue(gameObject);
		return true;
	}

	return false;
}

std::string ObjectDrawer::GetFieldName(
	const engine::Field& field,
	const std::vector<engine::Object>& attributes)
{
	auto predicate = [](auto attr)
	{
		return attr.GetTypeData() == engine::Types::kInspectorNameAttribute;
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

bool ObjectDrawer::IsEditableField(
	const engine::Field& field,
	const std::vector<engine::Object>& attributes)
{
	for (auto attr : attributes)
	{
		if (attr.GetTypeData() == engine::Types::kHideInInspectorAttribute)
		{
			return false;
		}
	}

	for (auto attr : attributes)
	{
		if (attr.GetTypeData() == engine::Types::kSerializeFieldAttribute)
		{
			return true;
		}
	}

	return field.IsPublic();
}

bool ObjectDrawer::TryGetSliderConstraints(
	const std::vector<engine::Object>& attributes,
	float& min_out,
	float& max_out)
{
	auto predicate = [](auto attr)
	{
		return attr.GetTypeData() == engine::Types::kSliderAttribute;
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


void ObjectDrawer::ChangeGameObjectResourcesCopasity(size_t size)
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


void ObjectDrawer::CopyAsNullTerminated(char* destination, const std::string& source)
{
	source.copy(destination, source.size());
	destination[source.size()] = '\0';
}
