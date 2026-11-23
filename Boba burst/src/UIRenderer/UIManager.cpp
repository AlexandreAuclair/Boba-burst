#include "UIManager.h"

#include <iostream>

UIAction UIManager::GetClickedAction(float mouseX, float mouseY, float halfWidth, float halfHeight) const {
	
	// ======================================================
    // Etape 1 : Tester si clic dans le menu
    // ======================================================
	if(!IsInsideMenu(mouseX, mouseY, halfWidth, halfHeight)) return UIAction::NONE;

	// ======================================================
    // 2. Calculer les limites du PNG à l'écran
    //    Coordonnées normalisées entre 0 et 1
    // ======================================================
    const float xMin = (1.0f - halfWidth) / 2.0f;
    const float xMax = (1.0f + halfWidth) / 2.0f;

    const float yMin = (1.0f - halfHeight) / 2.0f;
    const float yMax = (1.0f + halfHeight) / 2.0f;

    // ======================================================
    // 3. Convertir les coordonnées écran en UV du PNG
    //    (0,0) = coin supérieur gauche
    //    (1,1) = coin inférieur droit
    // ======================================================
    const float u = (mouseX - xMin) / (xMax - xMin);
    const float v = (mouseY - yMin) / (yMax - yMin);

    // Print debug
    std::cout
        << "Clic UV : u = " << u
        << " | v = " << v
        << std::endl;

    // ======================================================
    // 4. Définir les zones cliquables des boutons du PNG 500 x 500 - Coordonnées UV normalisées entre 0 et 1
    //    Valeurs provisoires à adapter au vrai PNG => Changer en fonction du PNG qu'on importe
    // ======================================================
   
    // Bouton Reprendre : en haut à droite
    const UIHitBox resumeButton{
        0.596f, 0.900f, 0.548f, 0.666f
    };

    // Bouton Redémarrer : en haut à gauche
    const UIHitBox restartButton{
        0.100f, 0.404f, 0.548f, 0.666f
    };

    // Bouton Quitter : en bas au centre
    const UIHitBox quitButton{
        0.348f, 0.652f, 0.756f, 0.874f
    };

    // ======================================================
    // 5. Identifier le bouton sur lequel on a cliqué
    // ======================================================
    if (IsInsideHitbox(u, v, resumeButton))
    {
        return UIAction::RESUME;
    }

    if (IsInsideHitbox(u, v, restartButton))
    {
        return UIAction::RESTART;
    }

    if (IsInsideHitbox(u, v, quitButton))
    {
        return UIAction::QUIT;
    }

    // Clic à l'intérieur du PNG, mais hors des boutons.
    return UIAction::NONE;
}


bool UIManager::IsInsideMenu(float mouseX, float mouseY, float halfWidth, float halfHeight) const {

	// Etape 1; Dimensions invalides => Evacuer le cas direct
	if (halfWidth <= 0 || halfHeight <= 0) return false;

	// Etape 2 :Rectangle centré coordonnées NDC [-1.0, 1.0] => Convertir vers coordonnées normalisées [0,1]
	// Calcul des bordures 
	// Calcul : x_ecran = (x_NDC + 1) / 2

	const float xMin = (1.0f - halfWidth) / 2.0f;
	const float xMax = (1.0f + halfWidth) / 2.0f;


	const float yMin = (1.0f - halfHeight) / 2.0f;
	const float yMax = (1.0f + halfHeight) / 2.0f;

	// Etape 3 : Curseur est-il à l'intérieur des quatres bords ?
	const bool insideX = mouseX >= xMin && mouseX <= xMax;

	const bool insideY = mouseY >= yMin && mouseY <= yMax;

	const bool result = insideX && insideY;

	return result;


}

bool UIManager::IsInsideHitbox(float u, float v, const UIHitBox& hitbox) const {
	const bool insideU = (u >= hitbox.uMin) && (u <= hitbox.uMax);
	const bool insideV = (v >= hitbox.vMin) && (v <= hitbox.vMax);

	const bool result = insideU && insideV;

	return result; 
}


