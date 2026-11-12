#pragma once

#include <memory>
#include <Jolt/Physics/Body/BodyInterface.h>

#include "bb_mesh.h"

struct Section {
    int id;
    int flags;
    std::unique_ptr<Section> prev;
    std::unique_ptr<Section> next;
    std::unique_ptr<Mesh> mesh;
    std::unique_ptr<JPH::BodyInterface> bodyInterface;
};