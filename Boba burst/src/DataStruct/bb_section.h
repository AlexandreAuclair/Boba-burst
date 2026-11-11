#pragma once

#include <memory>

#include "bb_mesh.h"

struct Section {
    int id;
    int flags;
    std::unique_ptr<Section> prev;
    std::unique_ptr<Section> next;
    std::unique_ptr<Mesh> mesh;
};