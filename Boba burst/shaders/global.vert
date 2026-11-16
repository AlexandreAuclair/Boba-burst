#version 460 core

layout(push_constant) uniform Matrices
{
    mat4 worldViewProjMatrix;
    mat4 worldMatrix;
} matrices;


// Entrees : position et normales des sommets + coordonnées textures (UV)
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

// Sortie : normales interpolees
layout(location = 0) out vec3 normalInterpolated;
layout(location = 1) out vec3 worldPosition;
layout (location = 2) out vec2 UVInterpolated;

void main()
{

// Sortie obligatoire pr�vue par GLSL => position dans le clip-space
    gl_Position =
        matrices.worldViewProjMatrix * vec4(inPosition, 1.0);

  // Position du sommet dans le monde => sert pour Blinn-Phong
    worldPosition = 
        ((matrices.worldMatrix) * vec4(inPosition,1.0)).xyz;

    normalInterpolated =
        normalize(mat3(matrices.worldMatrix) * inNormal);

    UVInterpolated = inUV;
}