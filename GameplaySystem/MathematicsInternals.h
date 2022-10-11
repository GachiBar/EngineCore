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

#pragma region Quaternion

//DirectX::SimpleMath::Quaternion Internal_LookRotation(DirectX::SimpleMath::Vector3 forward, DirectX::SimpleMath::Vector3 upwards) {
//	//DirectX::SimpleMath::Matrix look_at_matrix = DirectX::SimpleMath::Matrix::CreateLookAt(forward, upwards);
//	DirectX::SimpleMath::Quaternion r = DirectX::SimpleMath::Quaternion::Identity;
//	//auto e = r.ToEuler;
//	return DirectX::SimpleMath::Quaternion::CreateFromRotationMatrix(look_at_matrix);
//}

#pragma endregion Quaternion

void AddMathematicsInternalCalls() {	
	DirectX::SimpleMath::Quaternion q;

	mono_add_internal_call("GameplayCore.Mathematics.Vector3::ExternalMagnitude", Vector3Magnitude);
	mono_add_internal_call("GameplayCore.Mathematics.Vector3::ExternalMagnitudeSquared", Vector3MagnitudeSquared);
	mono_add_internal_call("GameplayCore.Mathematics.Vector3::ExternalNormolized", Vector3Normolized);
	mono_add_internal_call("GameplayCore.Mathematics.Vector3::ExternalSum", Vector3Sum);
}

} // namespace engine