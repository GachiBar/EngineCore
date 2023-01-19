#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>

JPH_SUPPRESS_WARNINGS

namespace engine {

struct CollisionLayers  : public JPH::ObjectLayerPairFilter {
	static constexpr JPH::uint8 kNoCollision = 0;
	static constexpr JPH::uint8 kNonMoving = 1;
	static constexpr JPH::uint8 kMoving = 2;
	static constexpr JPH::uint8 kLayersCount = 3;

	bool ShouldCollide(JPH::ObjectLayer object_layer_1, JPH::ObjectLayer object_layer_2);
};

} // namespace engine