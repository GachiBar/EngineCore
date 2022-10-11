#pragma once

#include <SimpleMath.h>
#include <mono/metadata/loader.h>

namespace engine {

#pragma region Quaternion

DirectX::SimpleMath::Quaternion Internal_FromToRotation(
	const DirectX::SimpleMath::Vector3& from_irection, 
	const DirectX::SimpleMath::Vector3& to_direction)
{
	return DirectX::SimpleMath::Quaternion::FromToRotation(from_irection, to_direction);
}

DirectX::SimpleMath::Quaternion Internal_Slerp(
	const DirectX::SimpleMath::Quaternion&  q1,
	const DirectX::SimpleMath::Quaternion&  q2,
	float t) 
{
	return DirectX::SimpleMath::Quaternion::Slerp(q1, q2, t);
}

DirectX::SimpleMath::Quaternion Internal_LookRotation(
	const DirectX::SimpleMath::Vector3& forward, 
	const DirectX::SimpleMath::Vector3& upwards)
{
	return DirectX::SimpleMath::Quaternion::LookRotation(forward, upwards);
}

DirectX::SimpleMath::Vector3 Internal_ToEulerRad(const DirectX::SimpleMath::Quaternion& q) {
	return q.ToEuler();
}

DirectX::SimpleMath::Quaternion Internal_FromEulerRad(const DirectX::SimpleMath::Vector3& euler) {
	return DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(euler);
}

#pragma endregion Quaternion

void AddMathematicsInternalCalls() {	
	mono_add_internal_call("GameplayCore.Mathematics.Quaternion::Internal_FromToRotation", Internal_FromToRotation);
	mono_add_internal_call("GameplayCore.Mathematics.Quaternion::Internal_Slerp", Internal_Slerp);
	mono_add_internal_call("GameplayCore.Mathematics.Quaternion::Internal_LookRotation", Internal_LookRotation);
	mono_add_internal_call("GameplayCore.Mathematics.Quaternion::Internal_ToEulerRad", Internal_ToEulerRad);
	mono_add_internal_call("GameplayCore.Mathematics.Quaternion::Internal_FromEulerRad", Internal_FromEulerRad);
}

} // namespace engine