// Boba burst.cpp : définit le point d'entrée de l'application.
//

#include "Game/bb_game.h"

using namespace std;

void main()
{
	Game game;

	game.Initialize();
	
	game.Run();

	game.Destroy();
}
