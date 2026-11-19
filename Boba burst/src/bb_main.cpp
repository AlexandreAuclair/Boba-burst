// Boba burst.cpp : définit le point d'entrée de l'application.
//

#include "Game/bb_game.h"


using namespace std;

int main()
{
	// 1. Récupérer la référence vers singleton
	Game& game = Game::get();
	
	// 2. Initialiser le jeu
	if(!game.Initialize()){
		return -1;
	}
	
	// 3. On fait rouler la boucle de jeu
	game.Run();


	// 4. On sort proprement
	game.Destroy();
}
