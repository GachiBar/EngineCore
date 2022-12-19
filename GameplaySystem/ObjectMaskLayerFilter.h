#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>

JPH_SUPPRESS_WARNINGS

namespace engine {

class ObjectMaskLayerFilter : public JPH::ObjectLayerFilter {
public:
	ObjectMaskLayerFilter(JPH::uint16 mask);

	bool ShouldCollide(JPH::ObjectLayer inLayer) const override;

private:
	JPH::uint16 mask_;
};

} // namespace engine