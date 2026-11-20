#version 450

// Entrée : UV coordinates du rectangle
layout(location = 0) in vec2 UVcoords;


// Sortie : couleur du pixel 
layout(location = 0) out vec4 outColor;


// Texture accessible via le descriptor set 0 au binding 0
layout(binding = 0, set = 0) uniform sampler2D uiTexture;

void main() {

	// Dégradé test passage position UV
	//outColor = vec4(UVcoords.x, UVcoords.y, 0.0, 1.0); 

	// Couleur du fragment une fois la texture binded et le descriptor set cree
	// texture() récupère couleur du PNG charge aux coordonnees UVcoords
	// outColor = texture(uiTexture, UVcoords);
	
	vec4 color = texture(uiTexture, UVcoords); 
	outColor = vec4(color.xyz, 1.0);

}
