#include "pch.h"
#include "CollisionLayers.h"

#include <cassert>

namespace engine {

bool CollisionLayers::IsCanCollide(JPH::ObjectLayer object_layer_1, JPH::ObjectLayer object_layer_2) {
	switch (object_layer_1) {
		case CollisionLayers::kNoCollision: {
			// No collision does not collide.
			return false;
		}
		case CollisionLayers::kNonMoving: {
			// Non moving only collides with moving.
			return object_layer_2 == CollisionLayers::kMoving;
		}
		case CollisionLayers::kMoving: {
			// Moving collides with everything
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