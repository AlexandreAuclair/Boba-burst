#version 460 core

// =====================================================
// VERTEX SHADER UI - BOBA BURST
// Objectif : dessiner un rectangle 2D en conservant
// les proportions de la texture, quelle que soit
// la résolution du framebuffer.
// =====================================================

// Entree : Push constant des infos

// Push Constants : dimensions du demi-rectangle en coordonnées NDC, calculées côté C++.
layout(push_constant) uniform UIParams
{
    float halfWidth;  // Demi-largeur NDC
    float halfHeight; // Demi-hauteur NDC
} uiParams;


// Sortie : Coordonnées UV transmises au Fragment Shader. Elles permettent d'échantillonner la texture PNG.
layout(location = 0) out vec2 UVcoords;

// Six sommets formant deux triangles.
//
// Ces positions décrivent un rectangle unité
// centré à l'origine, avant application de l'échelle.
//
// Les coordonnées varient de -1 à +1.
// Elles seront multipliées par les demi-dimensions.
const vec2 positions[6] = vec2[](
    // Premier triangle
    vec2(-1.0, -1.0), // Haut gauche
    vec2( 1.0, -1.0), // Haut droit
    vec2( 1.0,  1.0), // Bas droit

    // Deuxième triangle
    vec2(-1.0, -1.0), // Haut gauche
    vec2( 1.0,  1.0), // Bas droit
    vec2(-1.0,  1.0)  // Bas gauche
);


// UV associés aux six sommets.
// (0,0) = coin supérieur gauche de l'image.
// (1,1) = coin inférieur droit.
const vec2 UVCoords[6] = vec2[](
    
    // Premier triangle
    vec2( 0.0, 0.0),     // Bas gauche
    vec2( 1.0, 0.0),  // Bas droit
    vec2( 1.0,  1.0),  // Haut droit

    // Deuxième triangle
    vec2( 0.0, 0.0),     // Bas gauche
    vec2( 1.0, 1.0),  // Haut droit
    vec2(0.0,  1.0)   // Haut gauche
);


void main() {

    // ETAPE 1: Récupérer le sommet courant grâce à son indice.
    vec2 unitPosition = positions[gl_VertexIndex];

    // ETAPE 2 :Appliquer les demi-dimensions du rectangle.
    // => La multiplication vec2 * vec2 se fait composante par composante : (x*w, y*h).
    vec2 position = unitPosition *
        vec2(uiParams.halfWidth, uiParams.halfHeight);


    // ETAPE 3 : Transmettre UV au fragment shader
    UVcoords = UVCoords[gl_VertexIndex];

    // ETAPE 4 : Position finale du sommet dans le clip space
    gl_Position = vec4(position, 0.0, 1.0);
}