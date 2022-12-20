#pragma once

#include "Runtime.h"
#include "Method.h"
#include "Property.h"

#include <iostream>
#include <vector>
#include <mono/metadata/appdomain.h>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Collision/ContactListener.h>

JPH_SUPPRESS_WARNINGS

namespace engine {

struct BodyIdPair {
	JPH::uint32 body_id_1;
	JPH::uint32 body_id_2;
};

class EngineContactListener : public JPH::ContactListener {
public:
	bool IsAnyCollisionsEnter() {
		return !collisions_enter_.empty();
	}

	bool IsAnyCollisionsStay() {
		return !collisions_stay_.empty();
	}

	bool IsAnyCollisionsExit() {
		return !collisions_exit_.empty();
	}

	BodyIdPair PopCollisionsEnter() {
		auto result = collisions_enter_[collisions_enter_.size() - 1];
		collisions_enter_.pop_back();
		return result;
	}

	BodyIdPair PopCollisionsStay() {
		auto result = collisions_stay_[collisions_stay_.size() - 1];
		collisions_stay_.pop_back();
		return result;
	}

	BodyIdPair PopCollisionsExit() {
		auto result = collisions_exit_[collisions_exit_.size() - 1];
		collisions_exit_.pop_back();
		return result;
	}

	virtual void OnContactAdded(
		const JPH::Body& body_1, 
		const JPH::Body& body_2, 
		const JPH::ContactManifold& manifold, 
		JPH::ContactSettings& settings) override
	{
		JPH::uint32 body_id_1 = body_1.GetID().GetIndexAndSequenceNumber();
		JPH::uint32 body_id_2 = body_2.GetID().GetIndexAndSequenceNumber();
		collisions_enter_.emplace_back(body_id_1, body_id_2);
	}

	virtual void OnContactPersisted(
		const JPH::Body& body_1, 
		const JPH::Body& body_2, 
		const JPH::ContactManifold& manifold, 
		JPH::ContactSettings& settings) override
	{
		JPH::uint32 body_id_1 = body_1.GetID().GetIndexAndSequenceNumber();
		JPH::uint32 body_id_2 = body_2.GetID().GetIndexAndSequenceNumber();
		collisions_stay_.emplace_back(body_id_1, body_id_2);
	}

	virtual void OnContactRemoved(const JPH::SubShapeIDPair& sub_shape_pair) override
	{		
		JPH::uint32 body_id_1 = sub_shape_pair.GetBody1ID().GetIndexAndSequenceNumber();
		JPH::uint32 body_id_2 = sub_shape_pair.GetBody2ID().GetIndexAndSequenceNumber();
		collisions_exit_.emplace_back(body_id_1, body_id_2);
	}

private:
	std::vector<BodyIdPair> collisions_enter_;
	std::vector<BodyIdPair> collisions_stay_;
	std::vector<BodyIdPair> collisions_exit_;
};

} // namespace engine