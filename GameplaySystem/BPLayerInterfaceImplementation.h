#pragma once

#include "CollisionLayers.h"
#include "BroadPhaseLayers.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

#include <cassert>

JPH_SUPPRESS_WARNINGS
//#define JPH_PROFILE_ENABLED

namespace engine {

class BPLayerInterfaceImplementation final : public JPH::BroadPhaseLayerInterface
{
public:
	BPLayerInterfaceImplementation();

	virtual JPH::uint GetNumBroadPhaseLayers() const override;
	virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer layer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
	JPH::BroadPhaseLayer object_to_broad_phase_[CollisionLayers::kLayersCount];
};

} // namespace engine