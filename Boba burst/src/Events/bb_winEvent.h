#pragma once
#include "../EventBus/bb_event.h"

class WinEvent : public Event
{
public:
    WinEvent() {};
    /// @brief Événement étant appelé lorsqu'un joueur atteint la "ligne d'arrivée" 
    /// @param playerID joueur ayant effectué le contact avec la ligne d'arrivée
    WinEvent(int playerID)
    {
        winningPlayerID = playerID;
    };
    int winningPlayerID;
};