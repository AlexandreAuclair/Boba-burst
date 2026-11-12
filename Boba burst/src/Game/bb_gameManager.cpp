#include "bb_gameManager.h"

using namespace std;

GameManager::GameManager(){

}

GameManager::~GameManager(){

}

void GameManager::registerPlayer(std::unique_ptr<Player> p){
    players.push_back(move(p));
}

void GameManager::registerBot(std::unique_ptr<Bot> p){
    bots.push_back(move(p));
}

void GameManager::registerTrack(std::unique_ptr<Track> p){
    tracks.push_back(move(p));
}

void GameManager::registerTrackObject(std::unique_ptr<TrackObject> p){
    trackObjects.push_back(move(p));
}

void GameManager::update(){
    
}