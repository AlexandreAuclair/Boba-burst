// PAUSE_MENU_GEOMETRY_H => Calculer les demi-dimensions du menu pause

#pragma once

#include <cstdint>

// Dimensions d'un rectangle centré, exprimées en NDC.
// Ces valeurs sont également transmises au Vertex Shader.
struct UIPushConstants
{
    float halfWidth = 0.0f;
    float halfHeight = 0.0f;
};

// Calcule les dimensions du PNG à afficher sans déformation.
inline UIPushConstants CalculateTextureSize(
    uint32_t textureWidth,
    uint32_t textureHeight,
    uint32_t framebufferWidth,
    uint32_t framebufferHeight)
{
    // Éviter toute division par zéro.
    if (textureWidth == 0 || textureHeight == 0 ||
        framebufferWidth == 0 || framebufferHeight == 0)
    {
        return {};
    }

    // Le menu occupe 50 % de la hauteur du framebuffer.
    constexpr float halfHeight = 0.5f;

    // Rapport largeur / hauteur de la texture.
    const float textureRatio =
        static_cast<float>(textureWidth) /
        static_cast<float>(textureHeight);

    // Compensation du rapport d'aspect du framebuffer.
    const float framebufferRatio =
        static_cast<float>(framebufferHeight) /
        static_cast<float>(framebufferWidth);

    const float halfWidth =
        halfHeight * textureRatio * framebufferRatio;

    return { halfWidth, halfHeight };
}


