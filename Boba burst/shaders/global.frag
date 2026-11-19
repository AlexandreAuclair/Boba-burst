#version 460 core

// Entrée du vertex shader : normales interpolées et position sommet dans monde
layout(location = 0) in vec3 normalInterpolated;
layout(location = 1) in vec3 worldPosition;
layout (location = 2) in vec2 UVInterpolated;


// Sortie : La couleur du pixel
layout(location = 0) out vec4 outColor;


// Ma light directionnelle => Uniform buffer en lecture seule
// set = 0 : premier descriptor set.
// binding = 0 : premier binding dans ce set.
layout(set = 0, binding = 0, std140) uniform LightData
{
    vec4 direction;       // xyz : direction de la lumière (vers la scène)
    vec4 color;           // rgb : couleur de la lumière
    vec4 cameraPosition;  // xyz : position de la caméra en espace monde
} light;

// Ma texture pour le terrain
// binding = 1 : binding associé au sampler 2D créé dans CreateTexture() dans le CPP
layout(set = 0, binding = 1) uniform sampler2D terrainTexture;

void main()
{

    // Calcul de la lumière via Blinn-Phong 

    // ETAPE 0 : Calculs des différentes composantes pour Pixel shader
    vec3 N = normalize(normalInterpolated);
    vec3 L = normalize(-light.direction.xyz);
    vec3 V = normalize(light.cameraPosition.xyz - worldPosition);
    vec3 H = normalize (L + V);

    // Ne peut pas dépasser 1 car ils sont normalisées. Donc saturate revient à max(x,0)
    float NdotL = max(dot(N,L), 0);


    // Etape 1 : ambiant
    float ambiant = 0.20;

    // Etape 2 : diffus 
    float diffus = NdotL;

    // Etape 3 : Spéculaire
    float sp = 10;                                  // coeff puissance spéculaire
    

    // Déclaration préalable
    float specular = 0.0;

    // On ne fait la composante spéculaire que si la surface est éclairée
    if(NdotL > 0){
    specular = pow(max(dot(H,N), 0), sp);
    }

    // Etape 4 : Couleur finale du terrain => Echantillonner la texture(sampler2D, position_a_lire)
    vec3 terrainColor = texture(terrainTexture, UVInterpolated).rgb;

    vec3 ambiantTerm = ambiant * terrainColor * light.color.rgb;
    vec3 diffusTerm = diffus * terrainColor * light.color.rgb; 
    vec3 specularTerm = specular * light.color.rgb;                 // pas affecté par couleur terrain, uniquement par couleur lumière
    
    // Couleur finale du pixel
    vec3 finalColor = (ambiantTerm + diffusTerm + specularTerm);

    outColor = vec4(finalColor, 1.0);

}