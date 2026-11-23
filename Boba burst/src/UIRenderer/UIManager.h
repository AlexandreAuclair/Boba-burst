// UI_MANAGER_H => Checke quels boutons de l'UI sont cliqués et lance les actions en fonction

#pragma once

// Classe action associée aux boutons
enum class UIAction {
	NONE,
	QUIT,
	RESTART,
	RESUME
};


// Structure pour la hitbox des boutons avec coordonnees
struct UIHitBox {
	float uMin;
	float uMax;
	float vMin;
	float vMax;
};

class UIManager {

	
public : 

	/// <summary> Fonction stipulant si clic souris est dans le menu </summary>
	/// <param name="mouseX">Position en X du clic</param>
	/// <param name="mouseY">Position en Y du clic</param>
	/// <param name="halfWidth">Demi-largeur du rectangle</param>
	/// <param name="halfHeight">Demi-hauteur du rectangle</param>
	/// <returns>Booléen indiquant si le clic est dans le menu</returns>
	bool IsInsideMenu(float mouseX, float mouseY, float halfWidth, float halfHeight) const;

	/// <summary> </summary>
	/// <param name="u"></param>
	/// <param name="v"></param>
	/// <param name="hitbox"></param>
	/// <returns></returns>
	bool IsInsideHitbox(float u, float v, const UIHitBox& hitbox) const;


	/// <summary>
	/// 
	/// </summary>
	/// <param name="mouseX">X normalise de la souris</param>
	/// <param name="mouseY">Y normalise de la souris </param>
	/// <param name="halfWidth"></param>
	/// <param name="halfHeight"></param>
	/// <returns> Retourne l'action associee à la position du clic</returns>
	UIAction GetClickedAction(float mouseX,float mouseY,float halfWidth, float halfHeight) const;
};