#include "pch.h"
#include "BroadPhaseMaskLayerFilter.h"

namespace engine {

BroadPhaseMaskLayerFilter::BroadPhaseMaskLayerFilter(JPH::uint8 mask)
	: mask_(mask)
{}

bool BroadPhaseMaskLayerFilter::ShouldCollide(JPH::BroadPhaseLayer layer) const
{
	JPH::uint8 temp = (JPH::uint8)layer;
	return (mask_ & temp) == temp;
}

} // namespace engine