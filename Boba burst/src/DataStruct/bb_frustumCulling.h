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

    BoundingSphere GetWorldBoundingSphere(
        const BoundingSphere &localSphere,
        const glm::mat4 &modelMatrix)
    {
        glm::vec3 center = glm::vec3(
            modelMatrix *
            glm::vec4(localSphere.center, 1.0f));

        glm::vec3 scale(
            glm::length(glm::vec3(modelMatrix[0])),
            glm::length(glm::vec3(modelMatrix[1])),
            glm::length(glm::vec3(modelMatrix[2])));

        float maxScale = glm::max(
            scale.x,
            glm::max(scale.y, scale.z));

        return {
            center,
            localSphere.radius * maxScale};
    }
};
struct PlaneCulling
{
    glm::vec3 normal;
    float distance;
};

struct Frustum
{
    PlaneCulling planes[6];

    Frustum ExtractFrustum(const glm::mat4 &vp)
    {
        // Reconstruct matrix rows from GLM columns
        glm::vec4 r0(
            vp[0][0], vp[1][0],
            vp[2][0], vp[3][0]);

        glm::vec4 r1(
            vp[0][1], vp[1][1],
            vp[2][1], vp[3][1]);

        glm::vec4 r2(
            vp[0][2], vp[1][2],
            vp[2][2], vp[3][2]);

        glm::vec4 r3(
            vp[0][3], vp[1][3],
            vp[2][3], vp[3][3]);

        Frustum frustum;

        frustum.planes[0] = NormalizePlane(r3 + r0); // Left
        frustum.planes[1] = NormalizePlane(r3 - r0); // Right

        frustum.planes[2] = NormalizePlane(r3 + r1); // Bottom
        frustum.planes[3] = NormalizePlane(r3 - r1); // Top

        frustum.planes[4] = NormalizePlane(r2);      // Near
        frustum.planes[5] = NormalizePlane(r3 - r2); // Far

        return frustum;
    }

    PlaneCulling NormalizePlane(const glm::vec4 &p)
    {
        glm::vec3 normal = glm::vec3(p);

        float length = glm::length(normal);

        return {
            glm::normalize(normal),
            p.w / length};
    }
};