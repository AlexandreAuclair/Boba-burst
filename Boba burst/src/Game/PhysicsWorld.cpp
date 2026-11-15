#pragma once

#include "PhysicsWorld.h"



PhysicsWorld::PhysicsWorld() {

	mTempAllocator = new TempAllocatorImpl(32 * 1024 * 1024);

	// Create a factory, this class is responsible for creating instances of classes based on their name or hash and is mainly used for deserialization of saved data.
	// It is not directly used in this example but still required.
	Factory::sInstance = new Factory();

	// Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
	// If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
	// If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
	RegisterTypes();


	// We need a job system that will execute physics jobs on multiple threads. Typically
	// you would implement the JobSystem interface yourself and let Jolt Physics run on top
	// of your own job scheduler. JobSystemThreadPool is an example implementation.
	// 
	//JobSystemThreadPool job_system(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);
	mJobSystem = new JobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1); // declaree ailleurs

	physics_system.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);

	physics_system.SetBodyActivationListener(&body_activation_listener);

	physics_system.SetContactListener(&contact_listener);

	// ajout devoir : enlever gravité =========================
	physics_system.SetGravity(Vec3(0.0f, 0.0f, 0.0f));

	// -------------------- 

	// The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
// variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
	//body_interface = physics_system.GetBodyInterface();

	//*/

	// ---------------------------------------------------------------

};



bool PhysicsWorld::Update(float dt) 		
{

	//BodyInterface& body_interface = GetBodyInterface();

		if (dt != 0) {
			// TRAITEMENT DES ICARUS 
			/*while (!icarus_queue.empty()) {
				BodyID ic_id = icarus_queue.back(); icarus_queue.pop_back();
				body_interface.SetLinearVelocity(ic_id, Vec3(0.f, 50.f, 0.f));
			}*/



			//if (step % 60 == 0) {}

			// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
			const int cCollisionSteps = 1;

			// Step the world
			physics_system.Update(dt, cCollisionSteps, mTempAllocator, mJobSystem);
			//step++;

			//BodyManager::DrawSettings draw_settings;
			//draw_settings.mDrawShape = true;
			//physics_system.DrawBodies(draw_settings, mDebugRenderer);

		}

		return true;
};

//ENTRY_POINT(MyApp, RegisterDefaultAllocator)