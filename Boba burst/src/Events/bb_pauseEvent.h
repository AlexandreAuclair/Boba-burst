#include "../EventBus/bb_event.h"

class PauseEvent : public Event
{
public:
    PauseEvent() {};
    PauseEvent(bool status)
    {
        isPaused = status;
    };
    bool isPaused;
};