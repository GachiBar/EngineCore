#include "pch.h"
#include "CollisionLayers.h"

#include <cassert>

namespace engine {

bool CollisionLayers::IsCanCollide(JPH::ObjectLayer object_layer_1, JPH::ObjectLayer object_layer_2) {
	switch (object_layer_1) {
		case CollisionLayers::kNoCollision: {
			return false;
		}
		case CollisionLayers::kNonMoving: {
			return object_layer_2 == CollisionLayers::kMoving;
		}
		case CollisionLayers::kMoving: {
			return object_layer_2 != CollisionLayers::kNoCollision;
		}
		default: {
			assert(false);
			return false;
		}
	}
}

} // namespace engine