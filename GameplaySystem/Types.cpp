#include "pch.h"
#include "Types.h"

namespace engine {

// Components
const TypeData Types::kTransformComponent("GameplayCore.Components", "TransformComponent");

// Components types
const TypeData Types::kSingle("System", "Single");
const TypeData Types::kDouble("System", "Double");
const TypeData Types::kBoolean("System", "Boolean");
const TypeData Types::kSByte("System", "SByte");
const TypeData Types::kInt16("System", "Int16");
const TypeData Types::kInt32("System", "Int32");
const TypeData Types::kInt64("System", "Int64");
const TypeData Types::kByte("System", "Byte");
const TypeData Types::kUInt16("System", "UInt16");
const TypeData Types::kUInt32("System", "UInt32");
const TypeData Types::kUInt64("System", "UInt64");
const TypeData Types::kVector2("GameplayCore.Mathematics", "Vector2");
const TypeData Types::kVector3("GameplayCore.Mathematics", "Vector3");
const TypeData Types::kVector4("GameplayCore.Mathematics", "Vector4");
const TypeData Types::kString("System", "String");
const TypeData Types::kGameObject("GameplayCore", "GameObject");

// Attributes
const TypeData Types::kHideInInspectorAttribute("GameplayCore.Serialization", "HideInInspectorAttribute");
const TypeData Types::kSerializeFieldAttribute("GameplayCore.Serialization", "SerializeFieldAttribute");
const TypeData Types::kInspectorNameAttribute("GameplayCore.Editor", "InspectorNameAttribute");
const TypeData Types::kSliderAttribute("GameplayCore.Editor", "SliderAttribute");

} // namespace engine