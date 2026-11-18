#pragma once

#include <glm/glm.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <memory>

#include "../DataStruct/bb_mesh.h"
#include "../DataStruct/bb_transform.h"

class Actor{
public:
	Actor() = default;
	Actor(int id_, Transform t_) : id(id_), transform(t_){}
	~Actor() = default;
protected:
	// Methode pour creer et enregistrer le corps physique dans Jolt
	virtual JPH::BodyID InitPhysics(JPH::BodyInterface& bodyInterface) = 0;

	// Methode appelee a chaque frame pour synchroniser la position de l'acteur avec Jolt
	virtual void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) = 0;
public:
	virtual void Update(int dt) = 0;
	virtual void Render(int dt) = 0;

	Transform GetTransform() const { return transform; }

public:
	std::shared_ptr<Mesh> mesh;

protected:
	int id;
	Transform transform;

	bool isVisible;
	bool isSolid;
	JPH::BodyID bodyID;
};