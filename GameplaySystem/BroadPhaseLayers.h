#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

JPH_SUPPRESS_WARNINGS

namespace engine {

struct BroadPhaseLayers {
	static constexpr JPH::BroadPhaseLayer kNonMoving { 0 };
	static constexpr JPH::BroadPhaseLayer kMoving { 1 };
	static constexpr JPH::uint kLayersCount = 2;

	static bool IsCanCollide(JPH::ObjectLayer layer_1, JPH::BroadPhaseLayer layer_2);
};

} // namespace engine