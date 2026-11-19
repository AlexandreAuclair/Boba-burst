#version 450

// Sortie : couleur du pixel 
layout(location = 0) out vec4 outColor;


void main() {

	// Couleur orange opaque pour triangle 2D debug : R, G, B, alpha
	outColor = vec4(1.0, 0.5, 0.2, 1.0); 
}

