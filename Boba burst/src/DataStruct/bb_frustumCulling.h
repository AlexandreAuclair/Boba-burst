#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

struct BoundingSphere
{
    glm::vec3 center;
    float radius;
};
struct Plane
{
    glm::vec3 normal;
    float distance;

    Plane NormalizePlane(const glm::vec4 &p)
    {
        glm::vec3 normal = glm::vec3(p);

        float length = glm::length(normal);

        return {
            glm::normalize(normal),
            p.w / length};
    }
};

struct Frustum
{
    Plane planes[6];

    Frustum ExtractFrustum(const glm::mat4 &vp)
    {
        glm::vec4 r0 = glm::row(vp, 0);
        glm::vec4 r1 = glm::row(vp, 1);
        glm::vec4 r2 = glm::row(vp, 2);
        glm::vec4 r3 = glm::row(vp, 3);

        Frustum frustum;

        // Left
        frustum.planes[0] = NormalizePlane(r3 + r0);

        // Right
        frustum.planes[1] = NormalizePlane(r3 - r0);

        // Bottom
        frustum.planes[2] = NormalizePlane(r3 + r1);

        // Top
        frustum.planes[3] = NormalizePlane(r3 - r1);

        // Near (Vulkan: depth 0)
        frustum.planes[4] = NormalizePlane(r2);

        // Far (Vulkan: depth 1)
        frustum.planes[5] = NormalizePlane(r3 - r2);

        return frustum;
    }
};