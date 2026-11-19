#version 460 core
layout(location = 2) in vec2 UVInterpolated;
layout(location = 0) out vec4 outColor;
layout(set = 0, binding = 1) uniform sampler2D terrainTexture;

void main() {
    outColor = texture(terrainTexture, UVInterpolated);
}