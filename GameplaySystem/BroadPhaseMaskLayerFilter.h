#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

JPH_SUPPRESS_WARNINGS

namespace engine {

class BroadPhaseMaskLayerFilter : public JPH::BroadPhaseLayerFilter {
public:
	BroadPhaseMaskLayerFilter(JPH::uint8 mask);

	bool ShouldCollide(JPH::BroadPhaseLayer layer) const override;

private:
	JPH::uint8 mask_;
};

} // namespace engine