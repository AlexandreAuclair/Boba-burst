#pragma once
#include "../EventBus/bb_event.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Physics/Body/BodyInterface.h>

class GameMovementEvent : public Event
{
public:
    GameMovementEvent(JPH::BodyID bodyInMotion, bool movingLeft, bool movingRight)
    {
        bodyID = bodyInMotion;
        isLeftInput = movingLeft;
        isRightInput = movingRight;
    }

    JPH::BodyID bodyID;
    bool isLeftInput;
    bool isRightInput;
};