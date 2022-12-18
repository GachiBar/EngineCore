#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseQuery.h>

JPH_SUPPRESS_WARNINGS

class BroadPhaseMaskLayerFilter : public JPH::BroadPhaseLayerFilter {
public:
	BroadPhaseMaskLayerFilter(JPH::uint8 mask)
		: mask_(mask)
	{}

	bool ShouldCollide(JPH::BroadPhaseLayer layer) const override
	{
		JPH::uint8 temp = (JPH::uint8)layer;
		return (mask_ & temp) == temp;
	}

private:
	JPH::uint8 mask_;
};