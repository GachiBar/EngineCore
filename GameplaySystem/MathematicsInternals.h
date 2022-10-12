#pragma once

#include <SimpleMath.h>
#include <mono/metadata/loader.h>

namespace engine {

#pragma region Vector3

DirectX::SimpleMath::Vector3 Internal_TransformVector3(
	const DirectX::SimpleMath::Vector3& vector,
	const DirectX::SimpleMath::Matrix& matrix)
{
	return DirectX::SimpleMath::Vector3::Transform(vector, matrix);
}

#pragma endregion Vector3

#pragma region Vector4

DirectX::SimpleMath::Vector4 Internal_TransformVector4(
	const DirectX::SimpleMath::Vector4& vector,
	const DirectX::SimpleMath::Matrix& matrix)
{
	return DirectX::SimpleMath::Vector4::Transform(vector, matrix);
}

#pragma endregion Vector4

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

#pragma region Matrix

DirectX::SimpleMath::Matrix Internal_MatrixMultiply(
	const DirectX::SimpleMath::Matrix& m1,
	const DirectX::SimpleMath::Matrix& m2)
{
	return m1 * m2;
}

DirectX::SimpleMath::Matrix Internal_ModelMatrix(
	const DirectX::SimpleMath::Vector3& scale,
	const DirectX::SimpleMath::Quaternion& rotation,
	const DirectX::SimpleMath::Vector3& translation) 
{
	DirectX::SimpleMath::Matrix model = DirectX::SimpleMath::Matrix::Identity;
	model *= DirectX::SimpleMath::Matrix::CreateScale(scale);
	model *= DirectX::SimpleMath::Matrix::CreateFromQuaternion(rotation);
	model *= DirectX::SimpleMath::Matrix::CreateTranslation(translation);
	return model;
}

#pragma endregion Matrix

void AddMathematicsInternalCalls() {	
	// Vector3.
	mono_add_internal_call("GameplayCore.Mathematics.Vector3::Internal_TransformVector3", Internal_TransformVector3);

	// Vector4.
	mono_add_internal_call("GameplayCore.Mathematics.Vector4::Internal_TransformVector4", Internal_TransformVector4);

	// Quaternion.
	mono_add_internal_call("GameplayCore.Mathematics.Quaternion::Internal_FromToRotation", Internal_FromToRotation);
	mono_add_internal_call("GameplayCore.Mathematics.Quaternion::Internal_Slerp", Internal_Slerp);
	mono_add_internal_call("GameplayCore.Mathematics.Quaternion::Internal_LookRotation", Internal_LookRotation);
	mono_add_internal_call("GameplayCore.Mathematics.Quaternion::Internal_ToEulerRad", Internal_ToEulerRad);
	mono_add_internal_call("GameplayCore.Mathematics.Quaternion::Internal_FromEulerRad", Internal_FromEulerRad);

	// Matrix.
	mono_add_internal_call("GameplayCore.Mathematics.Matrix::Internal_MatrixMultiply", Internal_MatrixMultiply);
	mono_add_internal_call("GameplayCore.Mathematics.Matrix::Internal_ModelMatrix", Internal_ModelMatrix);
}

} // namespace engine