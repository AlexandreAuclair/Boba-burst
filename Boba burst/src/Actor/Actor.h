#pragma once

#include <glm/glm.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Physics/Body/BodyInterface.h>

class Mesh{};

class Actor{
public:
	Actor() = default;
	~Actor() = default;

	// Méthode pour créer et enregistrer le corps physique dans Jolt
	virtual void InitPhysics(JPH::BodyInterface& bodyInterface) = 0;

	// Méthode appelée à chaque frame pour synchroniser la position de l'acteur avec Jolt
	virtual void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) = 0;
protected:
	int id;
	glm::vec3 position;
	Mesh *mesh;

	bool isVisible;
	bool isSolid;
	JPH::BodyID bodyID;
};