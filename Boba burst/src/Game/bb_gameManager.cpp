#include "bb_gameManager.h"

using namespace std;

GameManager::GameManager()
{
    m_physicsWorld = std::make_unique<PhysicsWorld>();
}

GameManager::~GameManager()
{
}

void GameManager::registerPlayer(std::unique_ptr<Player> p)
{
    actorsById.insert({p->getId(), p.get()});
    players.push_back(move(p));
}

void GameManager::registerBot(std::unique_ptr<Bot> p)
{
    actorsById.insert({p->getId(), p.get()});
    bots.push_back(move(p));
}

void GameManager::registerTrack(std::unique_ptr<Track> p)
{
    actorsById.insert({p->getId(), p.get()});
    tracks.push_back(move(p));
}

void GameManager::registerTrackObject(std::unique_ptr<TrackObject> p)
{
    actorsById.insert({p->getId(), p.get()});
    trackObjects.push_back(move(p));
}

void GameManager::registerBonusObject(std::unique_ptr<BonusObject> p)
{
    actorsById.insert({p->getId(), p.get()});
    bonusObjects.push_back(move(p));
}

void GameManager::registerObstacle(std::unique_ptr<Obstacle> p)
{
    actorsById.insert({p->getId(), p.get()});
    obstacles.push_back(move(p));
}

void GameManager::update()
{
}
