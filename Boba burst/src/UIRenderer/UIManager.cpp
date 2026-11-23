#include "UIManager.h"

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