#pragma once

#include <glm/glm.hpp>

class Mesh{};

class Actor{
int id;
glm::vec3 position;
Mesh mesh;
//JPH::Body* body; //pour plus tard
bool isVisible;
bool isSolid;
};