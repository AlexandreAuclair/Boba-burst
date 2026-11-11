#include "bb_gameManager.h"

using namespace std;

GameManager::GameManager(){

}

GameManager::~GameManager(){

}

void GameManager::registerPlayer(Player* p){
    players.push_back(make_unique<Player>(p));
}

void GameManager::registerBot(Bot* p){
    bots.push_back(make_unique<Bot>(p));
}

void GameManager::registerTrack(Track* p){
    tracks.push_back(make_unique<Track>(p));
}

void GameManager::registerTrackObject(TrackObject* p){
    trackObjects.push_back(make_unique<TrackObject>(p));
}

void GameManager::update(){

}