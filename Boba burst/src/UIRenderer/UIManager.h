// UI_MANAGER_H => Checke quels boutons de l'UI sont cliqués et lance les actions en fonction

#pragma once


class UIManager {

public : 

	/// <summary> Fonction stipulant si clic souris est dans le menu </summary>
	/// <param name="mouseX">Position en X du clic</param>
	/// <param name="mouseY">Position en Y du clic</param>
	/// <param name="halfWidth">Demi-largeur du rectangle</param>
	/// <param name="halfHeight">Demi-hauteur du rectangle</param>
	/// <returns>Booléen indiquant si le clic est dans le menu</returns>
	bool IsInsideMenu(float mouseX, float mouseY, float halfWidth, float halfHeight) const;

};