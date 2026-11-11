#pragma once

#include "bb_character.h"

class Player : public Character {
public:
    Player();
    ~Player();

	void InitPhysics(JPH::BodyInterface& bodyInterface) override final;

	void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) override final;
};