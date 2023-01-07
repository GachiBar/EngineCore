#include "pch.h"
#include "BroadPhaseLayers.h"
#include "CollisionLayers.h"

#include <cassert>

namespace engine {

bool BroadPhaseLayers::IsCanCollide(JPH::ObjectLayer object_layer_1, JPH::BroadPhaseLayer layer_2) {
	switch (object_layer_1) {
		case CollisionLayers::kNoCollision: {
			return false;
		}
		case CollisionLayers::kNonMoving: {
			return layer_2 == BroadPhaseLayers::kMoving;
		}
		case CollisionLayers::kMoving: {
			return layer_2 != BroadPhaseLayers::kNoCollision;
		}
		default: {
			assert(false);
			return false;
		}
	}
}

} // namespace engine