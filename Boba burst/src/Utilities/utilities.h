#pragma once 

#include "../DataStruct/bb_mesh.h"
#include <Jolt/Jolt.h>
#include <glm/glm.hpp>
#include <cassert>

#include <glm/gtc/quaternion.hpp>
#include <Jolt/Math/Quat.h>

inline JPH::Array<JPH::Vec3> MeshToJoltVertices(const Mesh& m) {

	std::vector<Sommet> sommets = m.sommets;
	JPH::Array<JPH::Vec3> verticesJolt;
	verticesJolt.reserve(sommets.size());

	for (auto& s : sommets) 
		verticesJolt.push_back(JPH::Vec3(s.position.x, s.position.y, s.position.z));

	return verticesJolt;
}

inline JPH::Vec3 glmToJPH(glm::vec3 v) {
    return JPH::Vec3(v.x, v.y, v.z);
}

inline JPH::Quat glmEulerToJPHQuat(const glm::vec3& eulerDegrees)
{
	glm::vec3 rad = glm::radians(eulerDegrees);
    glm::quat qz = glm::angleAxis(rad.z, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::quat qy = glm::angleAxis(rad.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat qx = glm::angleAxis(rad.x, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::quat q = glm::normalize(qx * qy * qz);
    return JPH::Quat(-q.x, q.y, -q.z, q.w);
}

inline JPH::IndexedTriangleList MeshToJoltTriangles(const Mesh& m) {
    const std::vector<uint32_t>& indices = m.indexes; 
    assert(indices.size() % 3 == 0); 

    JPH::IndexedTriangleList indicesJolt;
    indicesJolt.reserve(indices.size() / 3);

    for (size_t i = 0; i + 2 < indices.size(); i += 3) {
        indicesJolt.emplace_back(indices[i], indices[i + 1], indices[i + 2]);
    }

    assert(indices.size() / 3 == indicesJolt.size());

    return indicesJolt;
}

inline JPH::VertexList  MeshToJoltVertexList(const Mesh& m) {

	std::vector<Sommet> sommets = m.sommets;
	JPH::VertexList  verticesJolt;
	verticesJolt.reserve(sommets.size());

	for (const auto& s : sommets)
		verticesJolt.push_back(JPH::Float3(s.position.x, s.position.y, s.position.z));

	return verticesJolt;
}
