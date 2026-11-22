
#pragma once

#include <glm/glm.hpp>
#include <array>

struct FrustumPlane
{
    glm::vec3 normal;
    float distance;

    float GetSignedDistance(const glm::vec3 &point) const
    {
        return glm::dot(normal, point) + distance;
    }
};

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;

    static AABB TransformBounds(
        const AABB& localBounds,
        const glm::mat4& worldMatrix)
    {
        AABB worldBounds;

        worldBounds.min = glm::vec3(
            std::numeric_limits<float>::max());

        worldBounds.max = glm::vec3(
            std::numeric_limits<float>::lowest());

        // Transformer les 8 coins de la boîte.
        for (int x = 0; x < 2; ++x)
        {
            for (int y = 0; y < 2; ++y)
            {
                for (int z = 0; z < 2; ++z)
                {
                    glm::vec3 corner(
                        x ? localBounds.max.x : localBounds.min.x,
                        y ? localBounds.max.y : localBounds.min.y,
                        z ? localBounds.max.z : localBounds.min.z);

                    glm::vec3 worldCorner =
                        glm::vec3(worldMatrix * glm::vec4(corner, 1.0f));

                    worldBounds.min = glm::min(
                        worldBounds.min, worldCorner);

                    worldBounds.max = glm::max(
                        worldBounds.max, worldCorner);
                }
            }
        }

        return worldBounds;
    }
};

class Frustum
{
public:
    enum PlaneIndex
    {
        LEFT = 0,
        RIGHT = 1,
        BOTTOM = 2,
        TOP = 3,
        NEAR = 4,
        FAR = 5
    };

    void Update(const glm::mat4 &vp)
    {
        // Extraction des lignes de la matrice ViewProjection
        glm::vec4 row0(vp[0][0], vp[1][0], vp[2][0], vp[3][0]);
        glm::vec4 row1(vp[0][1], vp[1][1], vp[2][1], vp[3][1]);
        glm::vec4 row2(vp[0][2], vp[1][2], vp[2][2], vp[3][2]);
        glm::vec4 row3(vp[0][3], vp[1][3], vp[2][3], vp[3][3]);

        // Extraction des 6 plans.
        // Convention de profondeur Vulkan : 0 à 1.
        glm::vec4 extractedPlanes[6] =
            {
                row3 + row0, // Left
                row3 - row0, // Right
                row3 + row1, // Bottom
                row3 - row1, // Top
                row2,        // Near
                row3 - row2  // Far
        };

        for (int i = 0; i < 6; ++i)
        {
            glm::vec3 normal(extractedPlanes[i]);

            float length = glm::length(normal);

            if (length > 0.00001f)
            {
                planes[i].normal = normal / length;
                planes[i].distance = extractedPlanes[i].w / length;
            }
        }
    }

    bool Intersects(const AABB &bounds) const
    {
        for (const FrustumPlane &plane : planes)
        {
            // Sommet de la boîte le plus éloigné
            // dans la direction de la normale.
            glm::vec3 positiveVertex;

            positiveVertex.x =
                plane.normal.x >= 0.0f
                    ? bounds.max.x
                    : bounds.min.x;

            positiveVertex.y =
                plane.normal.y >= 0.0f
                    ? bounds.max.y
                    : bounds.min.y;

            positiveVertex.z =
                plane.normal.z >= 0.0f
                    ? bounds.max.z
                    : bounds.min.z;

            // Si même ce sommet est à l'extérieur,
            // toute la boîte est à l'extérieur.
            if (plane.GetSignedDistance(positiveVertex) < 0.0f)
                return false;
        }

        return true;
    }

private:
    std::array<FrustumPlane, 6> planes{};
};
