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

    Frustum ExtractFrustum(const glm::mat4& vp)
    {
        glm::vec4 c0 = vp[0];
        glm::vec4 c1 = vp[1];
        glm::vec4 c2 = vp[2];
        glm::vec4 c3 = vp[3];

        Frustum frustum;

        frustum.planes[0] = NormalizePlane(c3 + c0); // Left
        frustum.planes[1] = NormalizePlane(c3 - c0); // Right

        frustum.planes[2] = NormalizePlane(c3 + c1); // Bottom
        frustum.planes[3] = NormalizePlane(c3 - c1); // Top

        frustum.planes[4] = NormalizePlane(c2);      // Near
        frustum.planes[5] = NormalizePlane(c3 - c2); // Far

        return frustum;
    }

    PlaneCulling NormalizePlane(const glm::vec4& p)
    {
        glm::vec3 normal = glm::vec3(p);

        float length = glm::length(normal);

        return {
            glm::normalize(normal),
            p.w / length };
    }
};