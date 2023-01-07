#include "ObjectDrawer.h"

#include <imgui/imgui.h>
#include <SimpleMath.h>

#include "Float3Data.h"
#include "TextureData.h"
#include "Resources/MetadataReader.h"

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
    , game_objects_capacity(0)
{
	//CacheArbitratorsNames();
}

ObjectDrawer::~ObjectDrawer()
{
	if (game_objects_capacity > 0)
	{
		for (size_t i = 0; i < game_objects_capacity; ++i)
		{
			delete[] game_objects_names[i];
		}

		delete[] game_objects_pointers;
		delete[] game_objects_names;
	}
}

bool ObjectDrawer::DrawObject(engine::Object& object, std::vector<std::string>& modifiedFields)
{
	modifiedFields.clear();
	auto type = object.GetType();
	bool isFieldChanged = false;

	while (!type.Is(engine::Types::kObject)) 
	{
		auto fields = type.GetFields();

		for (auto field : fields)
		{
			if (field.GetType().Is(engine::Types::kSingle))
			{
				isFieldChanged |= DrawFloatField(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kDouble))
			{
				isFieldChanged |= DrawDoubleField(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kBoolean))
			{
				isFieldChanged |= DrawBoolField(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kSByte))
			{
				isFieldChanged |= DrawByteField(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kInt16))
			{
				isFieldChanged |= DrawShortField(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kInt32))
			{
				isFieldChanged |= DrawIntField(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kInt64))
			{
				isFieldChanged |= DrawLongField(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kByte))
			{
				isFieldChanged |= DrawUByteField(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kUInt16))
			{
				isFieldChanged |= DrawUShortField(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kUInt32))
			{
				isFieldChanged |= DrawUIntField(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kUInt64))
			{
				isFieldChanged |= DrawULongField(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kVector2))
			{
				isFieldChanged |= DrawVector2Field(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kVector3))
			{
				isFieldChanged |= DrawVector3Field(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kVector4))
			{
				isFieldChanged |= DrawVector4Field(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kColor3))
			{
				isFieldChanged |= DrawColor3Field(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kColor4))
			{
				isFieldChanged |= DrawColor4Field(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kString))
			{
				isFieldChanged |= DrawStringField(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kGameObject))
			{
				isFieldChanged |= DrawGameObjectField(object, field);
				modifiedFields.push_back(field.GetName());
			}
			else if (field.GetType().Is(engine::Types::kResource) || 
				field.GetType().IsDerivedFrom(engine::Types::kResource))
			{
				isFieldChanged |= DrawResourceField(object, field);
				modifiedFields.push_back(field.GetName());
			}
			//else if (field.GetType().IsDerivedFrom(engine::Types::kAIArbitrator) &&
			//	!field.GetType().IsAbstract()) 
			//{
			//	isFieldChanged |= DrawArbitratorField(object, field);
			//	modifiedFields.push_back(field.GetName());
			//}
		}

		type = type.GetBaseType();
	}

	return isFieldChanged;
}

bool ObjectDrawer::DrawFloatField(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	auto value = monoObject.Unbox<float>();

	float min = 0;
	float max = 0;

	if (TryGetSliderConstraints(attributes, min, max))
	{
		if (ImGui::SliderFloat(fieldName.c_str(), &value, min, max))
		{
			field.SetValue(object, &value);
			return true;
		}
	}
	else
	{
		if (ImGui::InputFloat(fieldName.c_str(), &value))
		{
			field.SetValue(object, &value);
			return true;
		}
	}

	return false;
}

bool ObjectDrawer::DrawDoubleField(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	auto value = monoObject.Unbox<double>();

	if (ImGui::InputDouble(fieldName.c_str(), &value))
	{
		field.SetValue(object, &value);
		return true;
	}

	return false;
}

bool ObjectDrawer::DrawBoolField(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	bool* value = reinterpret_cast<bool*>(monoObject.Unbox());

	if (ImGui::Checkbox(fieldName.c_str(), value))
	{
		field.SetValue(object, value);
		return true;
	}

	return false;
}

bool ObjectDrawer::DrawByteField(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	void* value = monoObject.Unbox();
	ImS8 step = 1;

	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_S8, value, &step))
	{
		field.SetValue(object, value);
		return true;
	}

	return false;
}

bool ObjectDrawer::DrawShortField(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	void* value = monoObject.Unbox();
	ImS16 step = 1;

	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_S16, value, &step))
	{
		field.SetValue(object, value);
		return true;
	}

	return false;
}

bool ObjectDrawer::DrawIntField(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	void* value = monoObject.Unbox();
	ImS32 step = 1;

	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_S32, value, &step))
	{
		field.SetValue(object, value);
		return true;
	}

	return false;
}

bool ObjectDrawer::DrawLongField(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	void* value = monoObject.Unbox();
	ImS64 step = 1;

	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_S64, value, &step))
	{
		field.SetValue(object, value);
		return true;
	}

	return false;
}

bool ObjectDrawer::DrawUByteField(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	void* value = monoObject.Unbox();
	ImU8 step = 1;

	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_U8, value, &step))
	{
		field.SetValue(object, value);
		return true;
	}

	return false;
}

bool ObjectDrawer::DrawUShortField(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	void* value = monoObject.Unbox();
	ImU16 step = 1;

	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_U16, value, &step))
	{
		field.SetValue(object, value);
		return true;
	}

	return false;
}

bool ObjectDrawer::DrawUIntField(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	void* value = monoObject.Unbox();
	ImU32 step = 1;

	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_U32, value, &step))
	{
		field.SetValue(object, value);
		return true;
	}

	return false;
}

bool ObjectDrawer::DrawULongField(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	void* value = monoObject.Unbox();
	ImU64 step = 1;

	if (ImGui::InputScalar(fieldName.c_str(), ImGuiDataType_U64, value, &step))
	{
		field.SetValue(object, value);
		return true;
	}

	return false;
}

bool ObjectDrawer::DrawVector2Field(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	auto value = monoObject.Unbox<DirectX::SimpleMath::Vector2>();
	float vector[2] = { value.x, value.y };

	float min = 0;
	float max = 0;

	if (TryGetSliderConstraints(attributes, min, max))
	{
		if (ImGui::SliderFloat2(fieldName.c_str(), vector, min, max))
		{
			field.SetValue(object, vector);
			return true;
		}
	}
	else
	{
		if (ImGui::InputFloat2(fieldName.c_str(), vector))
		{
			field.SetValue(object, vector);
			return true;
		}
	}

	return false;
}

bool ObjectDrawer::DrawVector3Field(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	auto value = monoObject.Unbox<DirectX::SimpleMath::Vector3>();
	float vector[3] = { value.x, value.y, value.z };

	float min = 0;
	float max = 0;

	if (TryGetSliderConstraints(attributes, min, max))
	{
		if (ImGui::SliderFloat3(fieldName.c_str(), vector, min, max))
		{
			field.SetValue(object, vector);
			return true;
		}
	}
	else
	{
		if (ImGui::InputFloat3(fieldName.c_str(), vector))
		{
			field.SetValue(object, vector);
			return true;
		}
	}

	return false;
}

bool ObjectDrawer::DrawVector4Field(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	auto value = monoObject.Unbox<DirectX::SimpleMath::Vector4>();
	float vector[4] = { value.x, value.y, value.z, value.w };

	float min = 0;
	float max = 0;

	if (TryGetSliderConstraints(attributes, min, max))
	{
		if (ImGui::SliderFloat4(fieldName.c_str(), vector, min, max))
		{
			field.SetValue(object, vector);
			return true;
		}
	}
	else
	{
		if (ImGui::InputFloat4(fieldName.c_str(), vector))
		{
			field.SetValue(object, vector);
			return true;
		}
	}

	return false;
}

bool ObjectDrawer::DrawStringField(const engine::Object& object, engine::Field& field)
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
	auto mono_object = field.GetValue(object);

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
		field.SetValue(object, newValue);
		return true;
	}

	return false;
}

bool ObjectDrawer::DrawGameObjectField(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object);

	if (scene->Count() + 1 > game_objects_capacity)
	{
		auto degree = std::ceil(std::log2(scene->Count() + 1));
		ChangeGameObjectResourcesCapacity(std::pow(2, degree));
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
		field.SetValue(object, gameObject);
		return true;
	}

	return false;
}

bool ObjectDrawer::DrawResourceField(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object);

	FileType type = MetadataReader::GetTypeByClassName(field.GetType().GetName().c_str());
	
	cache_.update(MetadataReader::AssetsPath, type);
	
	int selected_index;
	if(monoObject.has_value())
		selected_index = cache_.get_index(monoObject.value().GetInternal());
	else
		selected_index = 0;
	
	if (ImGui::Combo(fieldName.c_str(), &selected_index, cache_.get_names_pointer(), cache_.size()))
	{
		auto resource = cache_.get_pointer(selected_index);

		if (resource != nullptr) 
		{
			field.SetValue(object, *resource);
		}
		else 
		{
			field.SetValue(object, nullptr);
		}

		return true;
	}
	
	return false;
}

bool ObjectDrawer::DrawColor3Field(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	auto value = monoObject.Unbox<Float3Data::Color>();
	float color_data[3] = { value.x, value.y, value.z };
	
	if (ImGui::ColorEdit3(fieldName.c_str(), color_data))
	{
		Float3Data::Color set_value[1] = {Float3Data::Color(color_data[0], color_data[1], color_data[2])};
		field.SetValue(object, set_value);
		return true;
	}

	return false;
}

bool ObjectDrawer::DrawColor4Field(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	auto fieldName = GetFieldName(field, attributes);
	auto monoObject = field.GetValue(object).value();
	auto value = monoObject.Unbox<float4>();
	float color_data[4] = { value.x, value.y, value.z, value.w};
	
	if (ImGui::ColorEdit4(fieldName.c_str(), color_data))
	{
		float4 set_value[1] = {float4( color_data[0], color_data[1], color_data[2], color_data[3])};
		field.SetValue(object, set_value);
		return true;
	}

	return false;
}

bool ObjectDrawer::DrawArbitratorField(const engine::Object& object, engine::Field& field)
{
	auto attributes = field.GetAttributes();

	if (!IsEditableField(field, attributes))
	{
		return false;
	}

	int selected = 0;
	auto currentArbitrator = field.GetValue(object);

	if (ImGui::Combo("Arbitrator", &selected, arbitrators_names, arbitrators_names_count)) 
	{

	}
	
	return false;
}

std::string ObjectDrawer::GetFieldName(
	const engine::Field& field,
	const std::vector<engine::Object>& attributes)
{
	auto predicate = [](auto attr)
	{
		return attr.GetType().Is(engine::Types::kInspectorNameAttribute);
	};

	auto nameAttribute = std::find_if(attributes.begin(), attributes.end(), predicate);

	if (nameAttribute == attributes.end())
	{
		return field.GetName();
	}

	auto nameProperty = nameAttribute->GetType().GetProperty("Name");
	auto value = nameProperty.GetValue(*nameAttribute).value();
	mono::mono_string result(value.GetInternal());
	return result.as_utf8();
}

bool ObjectDrawer::IsEditableField(
	const engine::Field& field,
	const std::vector<engine::Object>& attributes)
{
	for (auto attr : attributes)
	{
		if (attr.GetType().Is(engine::Types::kHideInInspectorAttribute))
		{
			return false;
		}
	}

	for (auto attr : attributes)
	{
		if (attr.GetType().Is(engine::Types::kSerializeFieldAttribute))
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
		return attr.GetType().Is(engine::Types::kSliderAttribute);
	};

	auto it = std::find_if(attributes.begin(), attributes.end(), predicate);

	if (it == attributes.end())
	{
		return false;
	}

	auto minProperty = it->GetType().GetProperty("Min");
	auto minValue = minProperty.GetValue(*it).value();
	min_out = minValue.Unbox<float>();

	auto maxProperty = it->GetType().GetProperty("Max");
	auto maxValue = maxProperty.GetValue(*it).value();
	max_out = maxValue.Unbox<float>();

	return true;
}


void ObjectDrawer::CacheArbitratorsNames()
{
	std::vector<std::string> arbitrators;
	arbitrators.push_back("None");
	auto& runtime = engine::Runtime::GetCurrentRuntime();
	auto baseComponentType = runtime.GetType(engine::Types::kAIArbitrator);
	auto typeNames = runtime.DumpTypeNames();

	for (auto typeName : typeNames)
	{
		try
		{
			auto typeDeclarartion = engine::Types::ParseFullName(typeName);
			auto type = engine::Runtime::GetCurrentRuntime().GetType(typeDeclarartion);

			while (type.HasBaseType() && !type.IsAbstract())
			{
				if (type.IsDerivedFrom(baseComponentType))
				{
					arbitrators.push_back(typeName);
					break;
				}

				type = type.GetBaseType();
			}
		}
		catch (mono::mono_exception& ex)
		{
			continue;
		}
	}

	arbitrators_names_count = arbitrators.size();
	arbitrators_names = new char* [arbitrators_names_count];
	char** temp = arbitrators_names;

	for (auto arbitrator : arbitrators) 
	{
		CopyAsNullTerminated(*temp, arbitrator);
		std::advance(temp, 1);
	}
}

void ObjectDrawer::ChangeGameObjectResourcesCapacity(size_t size)
{
	if (game_objects_capacity > 0)
	{
		for (size_t i = 0; i < game_objects_capacity; ++i)
		{
			delete[] game_objects_names[i];
		}

		delete[] game_objects_pointers;
		delete[] game_objects_names;
	}

	game_objects_capacity = size;
	game_objects_pointers = new void* [game_objects_capacity];
	game_objects_names = new char* [game_objects_capacity];

	for (size_t i = 0; i < game_objects_capacity; ++i)
	{
		game_objects_names[i] = new char[kGameObjectNameMaxSize];
	}
}


void ObjectDrawer::CopyAsNullTerminated(char* destination, const std::string& source)
{
	source.copy(destination, source.size());
	destination[source.size()] = '\0';
}

ObjectDrawer::resources_cache::resources_cache()
{
	files_path.push_back(std::make_pair(std::filesystem::path("none"), nullptr));
	resource_names.push_back("None\0");
}

ObjectDrawer::resources_cache::~resources_cache() {
	for (auto it : resource_names) {
		delete it;
	}
}

void ObjectDrawer::resources_cache::update(std::filesystem::path basepath, FileType type = Other)
{
	if (MetadataReader::calculate_assets_count(basepath) == files_path.size() + 1)
		return;
	
	auto iterator = MetadataReader::iterate_assets_recursive_by_type(basepath, type);

	int i;
	for (i = 1; iterator; i++)
	{
		FileData data = iterator();

		if (files_path.size() <= i)
		{
			files_path.push_back(std::make_pair(data.path, nullptr));

			auto file_name = data.filename();
			char* resource_name = new char[file_name.size() + 1];
			CopyAsNullTerminated(resource_name, file_name);

			resource_names.push_back(resource_name);
			printf(data.filename().c_str() + '\n');
			continue;
		}

		if (files_path[i].first != data.path)
		{
			files_path.insert(files_path.begin() + i, std::make_pair(data.path, nullptr));

			auto file_name = data.filename();
			char* resource_name = new char[file_name.size() + 1];
			CopyAsNullTerminated(resource_name, file_name);

			resource_names.insert(resource_names.begin() + i, resource_name);
		}
	}

	// Handle case with deleted resource
	files_path.resize(i);
}

std::string ObjectDrawer::resources_cache::get_name(int index) const
{
	assert(index > -1 && index < files_path.size());
	std::filesystem::path path = files_path[index].first;
	return path.filename().generic_string();
}

std::shared_ptr<engine::Object> ObjectDrawer::resources_cache::get_pointer(int index)
{
	assert(index > -1 && index < files_path.size());

	if (index == 0)
		return nullptr;

	// Call MetadataReader to get object by path string
	std::filesystem::path path = files_path[index].first;

	if (files_path[index].second != nullptr)
		return files_path[index].second;

	// Use Dotnet.MetadataReader.Read()
	std::optional<engine::Object> resource = MetadataReader::read_internal(path);

	if (resource.has_value()) 
	{
		auto result = std::make_shared<engine::Object>(resource.value());
		files_path[index].second = result;
		return result;
	}

	return nullptr;
}

int ObjectDrawer::resources_cache::get_index(const engine::Object& resource)
{
	// Get Filename of instance
	auto property = resource.GetType().GetProperty("FilePath");
	std::optional<engine::Object> propertyValue = property.GetValue(resource);

	if (!propertyValue.has_value())
		return -1;

	std::string pathString = mono::mono_string(propertyValue.value().GetInternal()).as_utf8();

	// Convert string to std::filesystem::path
	std::filesystem::path path(pathString);

	// Find equality
	for (int i = 1; i < files_path.size(); i++)
	{
		if (files_path[i].first == path)
		{
			if (files_path[i].second == nullptr || *files_path[i].second != resource)
				files_path[i].second = std::make_shared<engine::Object>(resource);

			return i;
		}
	}

	return -1;
}

int ObjectDrawer::resources_cache::size() const
{
	return static_cast<int>(files_path.size());
}
