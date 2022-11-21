#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>

JPH_SUPPRESS_WARNINGS

namespace engine {

struct CollisionLayers {
	static constexpr JPH::uint8 kNonMoving = 0;
	static constexpr JPH::uint8 kMoving = 1;
	static constexpr JPH::uint8 kLayersCount = 2;

	static bool IsCanCollide(JPH::ObjectLayer object_layer_1, JPH::ObjectLayer object_layer_2);
};

} // namespace engine