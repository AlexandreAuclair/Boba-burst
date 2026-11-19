#version 460 core

// Positions des 6 sommets formant 2 triangles.
// Coordonnées normalisées : X et Y entre -1 et 1.

const vec2 positions[6] = vec2[](
    // Premier triangle
    vec2(-0.5, -0.3),  // Bas gauche
    vec2( 0.5, -0.3),  // Bas droit
    vec2( 0.5,  0.3),  // Haut droit

    // Deuxième triangle
    vec2(-0.5, -0.3),  // Bas gauche
    vec2( 0.5,  0.3),  // Haut droit
    vec2(-0.5,  0.3)   // Haut gauche
);

void main() {

    // Récupérer la position du sommet courant
    vec2 position = positions[gl_VertexIndex];

    // Conversion en vec4 pour position finale
    gl_Position = vec4(position, 0.0, 1.0);
}