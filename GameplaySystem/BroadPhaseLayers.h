#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

JPH_SUPPRESS_WARNINGS

namespace engine {

struct BroadPhaseLayers  : public JPH::ObjectVsBroadPhaseLayerFilter {
	static constexpr JPH::BroadPhaseLayer kNoCollision { 0 };
	static constexpr JPH::BroadPhaseLayer kNonMoving { 1 };
	static constexpr JPH::BroadPhaseLayer kMoving { 2 };
	static constexpr JPH::uint kLayersCount = 3;

	virtual bool ShouldCollide(JPH::ObjectLayer object_layer_1, JPH::BroadPhaseLayer layer_2);
};

} // namespace engine