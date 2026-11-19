#pragma once
#include "../EventBus/bb_event.h"

class PauseEvent : public Event
{
public:
    PauseEvent() {};
    /// @brief Événement étant appeler lorsque le menu pause est ouver ou fermer
    /// @param status Est-ce que le nouvel état est une Pause ou un Résumer
    PauseEvent(bool status)
    {
        isPaused = status;
    };
    bool isPaused;
};