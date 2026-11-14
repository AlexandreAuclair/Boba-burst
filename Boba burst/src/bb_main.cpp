// Boba burst.cpp : définit le point d'entrée de l'application.
//

#include "Game/bb_game.h"

using namespace std;

int main()
{
	if(!Game::get().Initialize()){
		return -1;
	}
	
	Game::get().Run();

	Game::get().Destroy();
}
