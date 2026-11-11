#pragma once

#include "bb_character.h"

class Bot : public Character {
public:
    Bot();
    ~Bot();

	void InitPhysics(JPH::BodyInterface& bodyInterface) override final;

	void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) override final;

};