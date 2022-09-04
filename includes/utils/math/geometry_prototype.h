#pragma once

#include "resources/model.h"
#include "utils/math/vec3.h"
#include <vector>

Model getConvexHull(std::vector<Vec3f> vertices);