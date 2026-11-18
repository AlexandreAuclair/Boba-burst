#pragma once

// The Jolt headers don't include Jolt.h. Always include Jolt.h before including any other Jolt header.
// You can use Jolt.h in your precompiled header to speed up compilation.
#include <Jolt/Jolt.h>

// Jolt includes

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h> // added capsule shape
#include <Jolt/Physics/PhysicsStepListener.h>		   // added mPhysicsSystem
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include <Jolt/Core/JobSystemSingleThreaded.h>
#include <Jolt/Core/StreamWrapper.h>
#include <Jolt/Core/StringTools.h>
#include <Jolt/Geometry/OrientedBox.h>
#include <Jolt/Compute/CPU/ComputeSystemCPU.h>
#include <Jolt/Physics/StateRecorderImpl.h>
#include <Jolt/Physics/SoftBody/SoftBodyMotionProperties.h>
#include <Jolt/Physics/SoftBody/SoftBodyCreationSettings.h>
#include <Jolt/Physics/PhysicsScene.h>
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/ShapeCast.h>
#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Collision/CollidePointResult.h>
#include <Jolt/Physics/Collision/AABoxCast.h>
#include <Jolt/Physics/Collision/CollisionCollectorImpl.h>
#include <Jolt/Physics/Collision/Shape/HeightFieldShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Collision/Shape/TaperedCapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>
#include <Jolt/Physics/Collision/Shape/TaperedCylinderShape.h>
#include <Jolt/Physics/Collision/Shape/TriangleShape.h>
#include <Jolt/Physics/Collision/Shape/PlaneShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>
#include <Jolt/Physics/Collision/Shape/StaticCompoundShape.h>
#include <Jolt/Physics/Collision/Shape/MutableCompoundShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/EmptyShape.h>
#include <Jolt/Physics/Collision/NarrowPhaseStats.h>
#include <Jolt/Physics/Collision/CollideSoftBodyVertexIterator.h>
#include <Jolt/Physics/Constraints/DistanceConstraint.h>
#include <Jolt/Physics/Constraints/PulleyConstraint.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Shaders/HairWrapper.h>

// STL includes
#include <iostream>
#include <cstdarg>
#include <thread>

#include <mutex>
#include "../EventBus/bb_eventBus.h"
#include "../Events/bb_gameMovementEvent.h"
#include "../Events/bb_onCollisionEvent.h"

std::mutex mutexContacts;
std::mutex mutexMovement;

// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
JPH_SUPPRESS_WARNINGS

// All Jolt symbols are in the JPH namespace
using namespace JPH;

// If you want your code to compile using single or double precision write 0.0_r to get a Real value that compiles to double or float depending if JPH_DOUBLE_PRECISION is set or not.
using namespace JPH::literals;

// We're also using STL classes in this example
using namespace std;

namespace Layers // rajouter les layers nscrs =============================================
{
	static constexpr ObjectLayer NON_MOVING = 0;	 // track et décor
	static constexpr ObjectLayer TRACK_OBJECTS = 1;	 // objets "plats" sur la track | sensors
	static constexpr ObjectLayer PLAYER = 2;		 // véhicules = joueurs ou bots
	static constexpr ObjectLayer BONUS_OBJECTS = 3;	 // objet bonus sans "collision"
	static constexpr ObjectLayer OBSTACLES = 4;		 // obstacles avec "collision"
	static constexpr ObjectLayer TERRAIN_LIMITS = 5; // limites du terrain pour empêcher aux joueurs/bots/ etc de sortir
	static constexpr ObjectLayer NUM_LAYERS = 6;
};

// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
namespace BroadPhaseLayers
{
	static constexpr BroadPhaseLayer NON_MOVING(0);
	static constexpr BroadPhaseLayer MOVING(1);
	static constexpr uint NUM_LAYERS(2);
};

class PhysicsWorld
{

public:
	PhysicsWorld(std::unique_ptr<EventBus> &bus);
	bool Update(float dt);
	BodyInterface &GetBodyInterface() { return physics_system.GetBodyInterface(); }
	void HandleMovementEvent(GameMovementEvent &event);

private:
	const JPH::Vec3 impulseLeftValue = JPH::Vec3(0.0f, 0.0f, 5.0f);
	const JPH::Vec3 impulseRightValue = JPH::Vec3(0.0f, 0.0f, -5.0f);
	std::unique_ptr<EventBus> *busPtr;
	// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
	const uint cMaxBodies = 1024;

	// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
	const uint cNumBodyMutexes = 0;

	// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
	// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
	// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
	const uint cMaxBodyPairs = 1024;

	// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
	// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
	const uint cMaxContactConstraints = 1024;

	//---------------------------------------------------------------------------------

	// Create mapping table from object layer to broadphase layer
	// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
	// Also have a look at BroadPhaseLayerInterfaceTable or BroadPhaseLayerInterfaceMask for a simpler interface.

	// BroadPhaseLayerInterface implementation
	// This defines a mapping between object and broadphase layers.
	class BPLayerInterfaceImpl final : public BroadPhaseLayerInterface
	{
	public:
		BPLayerInterfaceImpl()
		{
			// Create a mapping table from object to broad phase layer
			// mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
			// mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;

			for (ObjectLayer l = 0; l < Layers::NUM_LAYERS; ++l) // ----------------------------------------------
				mObjectToBroadPhase[l] = BroadPhaseLayers::NON_MOVING;
			mObjectToBroadPhase[Layers::PLAYER] = BroadPhaseLayers::MOVING;
		}

		virtual uint GetNumBroadPhaseLayers() const override
		{
			return BroadPhaseLayers::NUM_LAYERS;
		}

		virtual BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer inLayer) const override
		{
			JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
			return mObjectToBroadPhase[inLayer];
		}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
		virtual const char *GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override
		{
			switch ((BroadPhaseLayer::Type)inLayer)
			{
			case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:
				return "NON_MOVING";
			case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:
				return "MOVING";
			default:
				JPH_ASSERT(false);
				return "INVALID";
			}
		}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

	private:
		BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
	};

	BPLayerInterfaceImpl broad_phase_layer_interface;

	//-------------------------------------------------------------------

	// Create class that filters object vs broadphase layers
	// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
	// Also have a look at ObjectVsBroadPhaseLayerFilterTable or ObjectVsBroadPhaseLayerFilterMask for a simpler interface.

	/// Class that determines if an object layer can collide with a broadphase layer
	class ObjectVsBroadPhaseLayerFilterImpl : public ObjectVsBroadPhaseLayerFilter
	{
	public:
		virtual bool ShouldCollide(ObjectLayer inLayer1, BroadPhaseLayer inLayer2) const override
		{
			// switch (inLayer1)
			//{
			// case Layers::NON_MOVING:
			//	return inLayer2 == BroadPhaseLayers::MOVING;
			//	//case Layers::MOVING:
			//	//	return true;

			// default:
			//	JPH_ASSERT(false);
			//	return false;
			// }

			return inLayer1 == Layers::PLAYER || inLayer2 == BroadPhaseLayers::MOVING;
		}
	};

	ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter;

	// -----------------------------------

	// Create class that filters object vs object layers
	// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
	// Also have a look at ObjectLayerPairFilterTable or ObjectLayerPairFilterMask for a simpler interface.

	class ObjectLayerPairFilterImpl : public ObjectLayerPairFilter
	{ /// Class that determines if two object layers can collide
	public:
		virtual bool ShouldCollide(ObjectLayer inObject1, ObjectLayer inObject2) const override
		{
			// switch (inObject1)
			//{
			//	//case Layers::NON_MOVING:
			//		//return inObject2 == Layers::MOVING || inObject2 == Layers::ICARUS; // Non moving only collides with moving or icarus
			//	//case Layers::MOVING:
			//	//	return true; // Moving collides with everything (alternative si on ignore les ghosts : Layers::MOVING || inObject2 == Layers::ICARUS || inObject2 == Layers::NON_MOVING; )

			// default:
			//	JPH_ASSERT(false);
			//	return false;
			// }

			return inObject1 == Layers::PLAYER || inObject2 == Layers::PLAYER;
		}
	};
	ObjectLayerPairFilterImpl object_vs_object_layer_filter;

	// Now we can create the actual physics system.
	PhysicsSystem physics_system;

	// -----------------------------------------------------------

	// A body activation listener gets notified when bodies activate and go to sleep
	// Note that this is called from a job so whatever you do here needs to be thread safe.
	// Registering one is entirely optional.

	// An example activation listener
	class MyBodyActivationListener : public BodyActivationListener
	{
	public:
		virtual void OnBodyActivated(const BodyID &inBodyID, uint64 inBodyUserData) override
		{
			cout << "A body got activated" << endl;
		}

		virtual void OnBodyDeactivated(const BodyID &inBodyID, uint64 inBodyUserData) override
		{
			cout << "A body went to sleep" << endl;
		}
	};

	MyBodyActivationListener body_activation_listener;

	// _____________________________________________________
public:
	struct ContactEvent
	{
		int acteur1, acteur2;

		/*friend std::ostream& operator<<(std::ostream& os, const ContactEvent& c) {
			os << "Contact : body 1" << c.body1.GetIndex() << ", body 2: " << c.body2.GetIndex() << ")";
			return os;
		}*/
	};

	struct ImpulsionRequest
	{
		BodyID mBodyID;
		JPH::Vec3 modsVec3;
	};

private:
	std::vector<ContactEvent> contactsQueue;
	std::vector<ImpulsionRequest> movementRequestsQueue;

public:
	std::vector<ContactEvent> GetContactsQueue()
	{
		return contactsQueue;
	}

private:
	// -----------------------------------------------

	// A contact listener gets notified when bodies (are about to) collide, and when they separate again.
	// Note that this is called from a job so whatever you do here needs to be thread safe.
	// Registering one is entirely optional.

	// An example contact listener
	class MyContactListener : public ContactListener
	{
		PhysicsWorld &currentWorld;

	public:
		MyContactListener(PhysicsWorld &w) : currentWorld{w} {};

		// See: ContactListener
		virtual ValidateResult OnContactValidate(const Body &b1, const Body &b2, RVec3Arg inBaseOffset, const CollideShapeResult &inCollisionResult) override
		{
			ContactEvent c{static_cast<const int>(b1.GetUserData()), static_cast<const int>(b2.GetUserData())};

			std::lock_guard(mutexContacts);
			currentWorld.contactsQueue.push_back(c);

			cout << "Contact added to the queue " << endl;

			// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
			return ValidateResult::AcceptAllContactsForThisBodyPair;
		}

		virtual void OnContactAdded(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override
		{

			cout << "A contact was added between" << (int)inBody1.GetObjectLayer() << "and" << (int)inBody2.GetObjectLayer() << endl;
		}

		virtual void OnContactPersisted(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override
		{
			// cout << "A contact was persisted" << endl;
		}

		virtual void OnContactRemoved(const SubShapeIDPair &inSubShapePair) override
		{
			// cout << "A contact was removed" << endl;
		}
	};

	MyContactListener contact_listener{*this};

	TempAllocator *mTempAllocator = nullptr; // Allocator for temporary allocations
	JobSystem *mJobSystem = nullptr;		 // The job system that runs physics jobs
};
