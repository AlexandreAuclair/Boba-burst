#version 460 core


// Sortie : Coordonnées UV transmises au fragment shader
layout(location = 0) out vec2 UVcoords;

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

    // Transmettre UV au fragment shader
    UVcoords = UVCoords[gl_VertexIndex];

    // Position finale du sommet dans le clip space
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}