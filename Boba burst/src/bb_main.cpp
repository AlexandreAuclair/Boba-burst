// Boba burst.cpp : définit le point d'entrée de l'application.
//

#include "Game/bb_game.h"

using namespace std;

int main()
{
	Game::get().Initialize();
	
	Game::get().Run();

	Game::get().Destroy();
}
