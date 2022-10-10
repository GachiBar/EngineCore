#pragma once

#include <SimpleMath.h>

namespace engine {

#pragma region Vector3

float Vector3Magnitude(DirectX::SimpleMath::Vector3 vector) {
	return vector.Length();
}

float Vector3MagnitudeSquared(DirectX::SimpleMath::Vector3 vector) {
	return vector.LengthSquared();
}

DirectX::SimpleMath::Vector3 Vector3Normolized(DirectX::SimpleMath::Vector3 vector) {
	vector.Normalize();
	return vector;
}

DirectX::SimpleMath::Vector3 Vector3Sum(DirectX::SimpleMath::Vector3 lhs, DirectX::SimpleMath::Vector3 rhs) {
	return lhs + rhs;
}

#pragma endregion Vector3

void AddMathematicsInternalCalls() {
	
	mono_add_internal_call("GameplayCore.Mathematics.Vector3::ExternalMagnitude", Vector3Magnitude);
	mono_add_internal_call("GameplayCore.Mathematics.Vector3::ExternalMagnitudeSquared", Vector3MagnitudeSquared);
	mono_add_internal_call("GameplayCore.Mathematics.Vector3::ExternalNormolized", Vector3Normolized);
	mono_add_internal_call("GameplayCore.Mathematics.Vector3::ExternalSum", Vector3Sum);
}

} // namespace engine