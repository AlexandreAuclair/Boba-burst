#include "PhysicsWorld.h"
#include <string>

PhysicsWorld::PhysicsWorld(std::unique_ptr<EventBus> &bus)
{
	RegisterDefaultAllocator();

	// Will Recieve requests from player to move physical object
	bus->SubscribeToEvent<GameMovementEvent>(this, &PhysicsWorld::HandleMovementEvent);
	busPtr = &bus;

	// Create a factory, this class is responsible for creating instances of classes based on their name or hash and is mainly used for deserialization of saved data.
	// It is not directly used in this example but still required.
	Factory::sInstance = new Factory();

	// Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
	// If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
	// If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
	RegisterTypes();

	mTempAllocator = new TempAllocatorImpl(32 * 1024 * 1024);

	// We need a job system that will execute physics jobs on multiple threads. Typically
	// you would implement the JobSystem interface yourself and let Jolt Physics run on top
	// of your own job scheduler. JobSystemThreadPool is an example implementation.
	//
	// JobSystemThreadPool job_system(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);
	mJobSystem = new JobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1); // declaree ailleurs

	physics_system.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);

	physics_system.SetBodyActivationListener(&body_activation_listener);

	physics_system.SetContactListener(&contact_listener);

	// DEBUG =========================================================

	/*
	JPH::BodyInterface& bi = physics_system.GetBodyInterface();

	// "joueur" : sphère dynamique, activée
	bi.CreateAndAddBody(JPH::BodyCreationSettings(
		new JPH::SphereShape(0.5f), JPH::RVec3(0.0f, 0.0f, 0.0f),
		JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::PLAYER),
		JPH::EActivation::Activate);

	// "bonus" : gros capteur statique au même endroit
	JPH::BodyCreationSettings s(
		new JPH::SphereShape(2.0f), JPH::RVec3(0.0f, 0.0f, 0.0f),
		JPH::Quat::sIdentity(), JPH::EMotionType::Static, Layers::BONUS_OBJECTS);
	s.mIsSensor = true;
	bi.CreateAndAddBody(s, JPH::EActivation::DontActivate);

	// DEBUG =========================================================

	//*/

	// ---------------------------------------------------------------
};

bool PhysicsWorld::Update(float dt)
{
	// Logger::Log("Update : dt = " + std::to_string(dt));

	if (dt != 0)
	{

		// ON TRAITE LES DEMANDES DE MOUVEMENTS
		JPH::BodyInterface &bodyInterface = physics_system.GetBodyInterface();

		for (size_t i = 0; i < movementRequestsQueue.size(); i++)
		{
			ImpulsionRequest currentMod = movementRequestsQueue[i];
			bodyInterface.GetLinearVelocity(currentMod.mBodyID);
			Logger::Log("Vitesse avant :" + std::to_string(bodyInterface.GetLinearVelocity(currentMod.mBodyID).GetX()));
			JPH::Vec3 newSpeed = bodyInterface.GetLinearVelocity(currentMod.mBodyID) + currentMod.modsVec3;
			JPH::Vec3 newRot = bodyInterface.GetAngularVelocity(currentMod.mBodyID) + currentMod.modsRotVec3;
			bodyInterface.SetLinearAndAngularVelocity(currentMod.mBodyID, newSpeed, newRot);
			Logger::Log("Vitesse apres :" + std::to_string(bodyInterface.GetLinearVelocity(currentMod.mBodyID).GetX()));
		}

		movementRequestsQueue.clear();

		// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
		const int cCollisionSteps = 1;

		// Step the world
		physics_system.Update(dt, cCollisionSteps, mTempAllocator, mJobSystem);
		// step++;
		// Logger::Log("Contact Queue size : " + std::to_string(contactsQueue.size()));

		// for (auto collision : contactsQueue)
		// {
		// 	busPtr->get()->EmitEvent<CollisionEvent>(collision.acteur1, collision.acteur2);
		// }

		std::vector<ContactEvent> events;
		{
			std::lock_guard<std::mutex> lock(mutexContacts);
			events.swap(contactsQueue);
		}
		for (auto &c : events)
			busPtr->get()->EmitEvent<CollisionEvent>(c.acteur1, c.acteur2);

		// BodyManager::DrawSettings draw_settings;
		// draw_settings.mDrawShape = true;
		// physics_system.DrawBodies(draw_settings, mDebugRenderer);
	}

	return true;
}
void PhysicsWorld::HandleMovementEvent(GameMovementEvent &event)
{
	ImpulsionRequest i;
	i.mBodyID = event.bodyID;
	i.modsVec3 = (event.isLeftInput == true ? impulseLeftValue : impulseRightValue);
	i.modsRotVec3 = (event.isLeftInput == true ? impulseRotLeftValue : impulseRotRightValue);
	std::lock_guard<std::mutex> lock(mutexMovem);
	movementRequestsQueue.push_back(i);
};

// ENTRY_POINT(MyApp, RegisterDefaultAllocator)