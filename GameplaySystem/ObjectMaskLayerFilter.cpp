#include "pch.h"
#include "ObjectMaskLayerFilter.h"

namespace engine {

ObjectMaskLayerFilter::ObjectMaskLayerFilter(JPH::uint16 mask)
	: mask_(mask)
{}

bool ObjectMaskLayerFilter::ShouldCollide(JPH::ObjectLayer layer) const
{
	JPH::uint16 temp = (JPH::uint16)layer;
	return (mask_ & temp) == temp;
}

} // namespace engine