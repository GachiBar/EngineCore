#include "pch.h"
#include "BPLayerInterfaceImplementation.h"

#include <cassert>

namespace engine {

BPLayerInterfaceImplementation::BPLayerInterfaceImplementation() {
	// Create a mapping table from object to broad phase layer
	object_to_broad_phase_[CollisionLayers::kNoCollision] = BroadPhaseLayers::kNoCollision;
	object_to_broad_phase_[CollisionLayers::kNonMoving] = BroadPhaseLayers::kNonMoving;
	object_to_broad_phase_[CollisionLayers::kMoving] = BroadPhaseLayers::kMoving;
}

JPH::uint BPLayerInterfaceImplementation::GetNumBroadPhaseLayers() const {
	return BroadPhaseLayers::kLayersCount;
}

JPH::BroadPhaseLayer BPLayerInterfaceImplementation::GetBroadPhaseLayer(JPH::ObjectLayer layer) const {
	assert(layer < CollisionLayers::kLayersCount);
	return object_to_broad_phase_[layer];
}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
const char* BPLayerInterfaceImplementation::GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const {
	using namespace JPH;
	switch ((BroadPhaseLayer::Type)layer) {
		case (BroadPhaseLayer::Type)BroadPhaseLayers::kNoCollision: {
			return "kNoCollision";
		}
		case (BroadPhaseLayer::Type)BroadPhaseLayers::kNonMoving:	{
			return "kNonMoving";
		}				
		case (BroadPhaseLayer::Type)BroadPhaseLayers::kMoving: {
			return "kMoving";
		}				
		default: {
			assert(false);
			return "INVALID";
		}
	}
	return "";
}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

} // namespace engine