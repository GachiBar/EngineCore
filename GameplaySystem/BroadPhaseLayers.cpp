#include "pch.h"
#include "BroadPhaseLayers.h"
#include "CollisionLayers.h"

#include <cassert>

namespace engine {

bool BroadPhaseLayers::IsCanCollide(JPH::ObjectLayer layer_1, JPH::BroadPhaseLayer layer_2) {
	switch (layer_1) {
		case CollisionLayers::kNoCollision: {
			// No collision does not collide.
			return false;
		}
		case CollisionLayers::kNonMoving: {
			// Non moving only collides with moving.
			return layer_2 == BroadPhaseLayers::kMoving;
		}
		case CollisionLayers::kMoving: {
			// Moving collides with everything.
			return true;
		}
		default: {
			// TODO: our own assertion.
			assert(false);
			return false;
		}
	}
}

} // namespace engine